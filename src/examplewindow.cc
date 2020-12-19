#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <list>
#include <cmath>
#include <cstring>
#include <dirent.h>
#include <utils/properties.h>
#include <glib.h>
#include <glibmm/ustring.h>
#include <glibmm/refptr.h>
#include <giomm/file.h>
#include <gtkmm/cssprovider.h>
#include <gdkmm/display.h>
#include <gdkmm/screen.h>
#include <ui.inc>
#include <iomanip>
#include "examplewindow.h"
#include "ui/hextextview.h"
#include <project/project.h>

using namespace std;
using namespace utils;
using namespace project;
using namespace Glib;
using namespace Gio;

Gtk::Window* ExampleWindow::_pAbout = 0;

ExampleWindow::ExampleWindow() : _app(nullptr)
{
	_app = new APP_DETAIL;
	_app->tools = new DETAIL_WINDOWS;
	_app->path = Glib::get_user_data_dir();

	_app->path.append("/hexlab");
	_app->app_file = _app->path + "/hexlab.prp";
	_app->res_path = "/usr/share/hexlab";
	if(!Glib::file_test(_app->app_file.c_str(), Glib::FILE_TEST_EXISTS))
        create_props_file();
    _app->props = new Properties(_app->app_file.c_str(), MODE::PRP_READWRITE);
	get_window_geometry();
	set_title(APP_NAME);
	set_border_width(1);
	add_iconset();

	cout << _app->path.c_str() << endl;
	Glib::RefPtr<Gtk::Builder> _refBuilder = Gtk::Builder::create();
	_refActionGroup = Gtk::ActionGroup::create();
	_refActionGroup->add(Gtk::Action::create("FileMenu", "_File"));

	_refActionGroup->add(Gtk::Action::create("FileOpen",
		Gtk::Stock::OPEN, "_Open", "Open an existing document"),
      sigc::mem_fun(*this, &ExampleWindow::on_file_open));

	_refActionGroup->add(Gtk::Action::create("FileNew",
		Gtk::Stock::NEW, "_New", "Create a new document"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_file_new_generic));

	_refActionGroup->add(Gtk::Action::create("FileClose",
		"Close", "Close the file currently in view"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_close));

	_refActionGroup->add(Gtk::Action::create("FileSave",
		Gtk::Stock::SAVE, "_Save", "Save the currently active document"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("FileSaveAs",
		"Save As...", "Save the currently active document under a new file name"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("file1",
		"file1", "Reopen this file"), Gtk::AccelKey("<control>1"),
		sigc::mem_fun(*this, &ExampleWindow::on_file1_accel));

	_refActionGroup->add(Gtk::Action::create("file2",
		"file2", "Reopen this file"), Gtk::AccelKey("<control>2"),
		sigc::mem_fun(*this, &ExampleWindow::on_file2_accel));

	_refActionGroup->add(Gtk::Action::create("file3",
		"file3", "Reopen this file"), Gtk::AccelKey("<control>3"),
		sigc::mem_fun(*this, &ExampleWindow::on_file3_accel));

	_refActionGroup->add(Gtk::Action::create("file4",
		"file4", "Reopen this file"), Gtk::AccelKey("<control>4"),
		sigc::mem_fun(*this, &ExampleWindow::on_file4_accel));

	_refActionGroup->add(Gtk::Action::create("FileQuit",
		Gtk::Stock::QUIT, "_Quit", "Exit the application"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_file_quit));

	_refActionGroup->add(Gtk::Action::create("EditMenu", "_Edit"));

	_refActionGroup->add(Gtk::Action::create("UndoEdit",
		Gtk::Stock::UNDO, "_Undo last action", "Undoes the last action"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("RedoEdit",
		Gtk::Stock::REDO, "_Redo last action", "Redo the last action"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("EditCopy",
		Gtk::Stock::COPY, "_Copy", "Copy data into edit buffer"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("EditPaste",
		Gtk::Stock::PASTE, "_Paste", "Paste data from edit buffer to point"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("DiskMenu", "_Disk"));

	_refActionGroup->add(Gtk::Action::create("DiskOpen",
		Gtk::Stock::HARDDISK, "_Open Filesystem...",
		"Open a file system for sector editing"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("DiskCopy",
		Gtk::Stock::COPY, "_Copy Sectors...",
		"Copy sectors between file systems"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("OptionsMenu", "_Options"));
	_refActionGroup->add(Gtk::Action::create("ByteOrderMenu", "_Byte Order"));
	Gtk::RadioAction::Group group_userlevel;

	_refChoiceOne = Gtk::RadioAction::create(group_userlevel,
		"LittleEndian", "Little Endian",
		"Displays the byte order of words in Intel format");

	_refActionGroup->add(_refChoiceOne,
		sigc::mem_fun(*this, &ExampleWindow::on_menu_choices_one));

	_refChoiceTwo = Gtk::RadioAction::create(group_userlevel,
		"BigEndian", "Big Endian",
		"Displays the byte order of words in Motorola format");

	_refActionGroup->add(_refChoiceTwo,
		sigc::mem_fun(*this, &ExampleWindow::on_menu_choices_two));

	_refActionGroup->add(Gtk::Action::create("FileOffsetMenu",
		"_File Offset"));
	Gtk::RadioAction::Group group_offset;

	_refOffsetHex = Gtk::RadioAction::create(group_offset,
		"FileOffsetHex", "Hex", "Display offsets in hexidecimal format");
	_refActionGroup->add(_refOffsetHex,
		sigc::mem_fun(*this, &ExampleWindow::on_menu_choices_one));

	_refOffsetDec = Gtk::RadioAction::create(group_offset,
		"FileOffsetDec", "Decimal", "Display offsets in decimal format");
	_refActionGroup->add(_refOffsetDec,
		sigc::mem_fun(*this, &ExampleWindow::on_menu_choices_two));

	bool bTest;
	(static_cast<bool>(_app->props->Get("importfrom", "0")))? bTest = true : bTest = false;

	_refActionGroup->add(Gtk::ToggleAction::create("ShowToolbar",
		"Show Toolbar", "Toggles display of the toolbar", bTest),
		sigc::mem_fun(*this, &ExampleWindow::on_toggle_toolbar));

	(static_cast<bool>(_app->props->Get("inspect", "1")))? bTest = true : bTest = false;
	_refActionGroup->add(Gtk::ToggleAction::create("ShowInspector",
		"Data Inspector", "Toggles data inspector view", bTest),
		sigc::mem_fun(*this, &ExampleWindow::on_toggle_datains));

	(static_cast<bool>(_app->props->Get("results", "1")))? bTest = true : bTest = false;
	_refActionGroup->add(Gtk::ToggleAction::create("ShowResults",
		"Results Window", "Toggles results window view", bTest),
		sigc::mem_fun(*this, &ExampleWindow::on_toggle_results));

	_refActionGroup->add(Gtk::Action::create("OptOptions",
		Gtk::Stock::PREFERENCES, "_Preferences",
		"Presents the preferences dialog"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_options));

	_refActionGroup->add(Gtk::Action::create("clrmru",
		"Clear MRU", "Clear MRU List"),
		sigc::mem_fun(*this, &ExampleWindow::ClearMRU));

	Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	_refActionGroup->add(Gtk::Action::create("ToolsMenu", "_Tools"));
	_refActionGroup->add(Gtk::Action::create("ToolsOps", "_Operations"));

	_refActionGroup->add(Gtk::Action::create("flip",
		Gtk::StockID("FLIP"), "Byte Flip...", "Byte Flip"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("inv",
		Gtk::StockID("INV"), "Inverse Bits", "Inverse Bits"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("shleft",
		"Shift Left...", "Perform a shift left on the selected bits"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("shright",
		"Shift Right...", "Perform a shift right on the selected bits"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("rotleft",
		"Rotate Left...", "Perform a rotate left on the selected bits"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("rotright",
		"Rotate Right...", "Perform a rotate right on the selected bits"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("blleft",
		"Block Shift Left...", "Shift the selected block of bits left"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("blright",
		"Block Shift Right...", "Shift the selected block of bits right"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("setceil",
		"Set Ceiling Value...", "Set a maximum value for a shift operation"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("setfloor",
		"Set Floor Value...", "Set a minimum value for a shift operation"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("and", "AND...",
		"Perform a logical AND on the selected bits"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("or", "OR...",
		"Perform a logical OR on the selected bits"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("xor", "XOR...",
		"Perform a logical XOR on the selected bits"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("change",
		"Change sign...", "Change the sign bit of the selected bit group"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("add",
		"Add...", "Add selected bit groups"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("sub",
		"Subtract...", "Subtract selected bit groups"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("mul",
		"Multiply...", "Multiply selected bit groups"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("div",
		"Divide...", "Divide selected bit groups"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("upper",
		"Upper Case", "Change the selected chars to upper case"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("lower",
		"Lower Case", "Change the selected chars to lower case"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("invcse",
		"Inverse Case", "Invert the case of the selected chars"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("ToolsMrks", Gtk::StockID("BOOK"),  "_Bookmarks"));

  	_refActionGroup->add(Gtk::Action::create("Generate",
		Gtk::StockID("CHK"),  "_Generate Checksum...",
		"Generate a checksum on the file in the current view"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("addmrk",
		"_Add Bookmark", "Add a new bookmark"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("nextmrk",
		"_Next Bookmark", "Move to next bookmark"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("prevmrk",
		"_Previous Bookmark", "Move to previous bookmark"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("gotomrk",
		"_Goto Selected Bookmark", "Set the cursor position to the start of this bookmark"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("seldata",
		"_Select Bookmark Data", "Select data specified by this bookmark"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("editmrk",
		"_Edit Bookmark", "Edit an existing bookmark"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("remmrk",
		"_Remove Bookmark", "Remove the selected bookmark"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("opencoll",
		"_Open Bookmark Collections", "Open bookmark collection"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("save", "_Save", "Save bookmarks to file"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

  	_refActionGroup->add(Gtk::Action::create("saveas",
		"Save _As...", "Save current bookmarkk collection to new file"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

  	_refActionGroup->add(Gtk::Action::create("clrmrks",
		"C_lear Bookmarks", "Clear all bookmarks"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

  	_refActionGroup->add(Gtk::Action::create("colprops",
		"Collection _Properties...", "Displays properties for the bookmark collection"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("distrib",
		"Character _Distribution", "View the distribution of characters"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("comp", "Com_pare"));
 	_refActionGroup->add(Gtk::Action::create("files",
		"_Compare Files", "Load the compare tool"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("recomp",
		"_Recompare", "Recmpare files"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

  	_refActionGroup->add(Gtk::Action::create("clrres",
		"C_lear Results", "Clear the previous compare results"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

  	_refActionGroup->add(Gtk::Action::create("next",
		"_Next Difference", "Displays the next comparison difference"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

  	_refActionGroup->add(Gtk::Action::create("prev",
		"_Previous Difference", "Displays the previous comparison difference"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

  	_refActionGroup->add(Gtk::Action::create("Generate",
		Gtk::StockID("CHK"),  "_Generate Checksum...",
		"Generate a checksum on the file in the current view"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

  	_refActionGroup->add(Gtk::Action::create("FindStr",
		"_Find Strings...", "Searches the document for all ascii and unicode strings"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

  	_refActionGroup->add(Gtk::Action::create("StructVwer",
		"Structure _Viewer"));

	_refActionGroup->add(Gtk::Action::create("selstruct",
		"_Select Structure Library", "Select a library of structures"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("clselib",
		"C_lose Library", "Close the working structure library"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("addstruct",
		"_Add Structure", "Add a structure from the working structure library"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

 	_refActionGroup->add(Gtk::Action::create("remstruct",
		"_Remove Structure", "Remove the selected structure"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

 	_refActionGroup->add(Gtk::Action::create("clrstruct",
		"_Clear Structures", "Clear all structures"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

 	_refActionGroup->add(Gtk::Action::create("lockstruct",
		"L_ock Structure", "Lock structure to document offset"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("fltstruct",
		"_Float Structure", "Float structure across documents/offsets"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("gotoaddr",
		"_Goto Address", "Go to the address of the selected "),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("HelpMenu",
		"_Help", "Displays the help file"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("AppHelp",
		Gtk::Stock::HELP, "Contents", "Displays the application documentation"),
		sigc::mem_fun(*this, &ExampleWindow::on_menu_others));

	_refActionGroup->add(Gtk::Action::create("HelpAbout",
		Gtk::Stock::ABOUT, "About...", "Application credits"),
		sigc::mem_fun(*this, &ExampleWindow::on_app_about));

	_refUIManager = Gtk::UIManager::create();
	_refUIManager->insert_action_group(_refActionGroup);
	add_accel_group(_refUIManager->get_accel_group());

	_refUIManager->signal_connect_proxy().
		connect(sigc::mem_fun(*this,
		&ExampleWindow::setup_statusbar_tooltip));

#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try {
		_refUIManager->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex) {
		cerr << "building menus failed: " <<  ex.what();
	}
#else
	unique_ptr<Glib::Error> ex;
	_refUIManager->add_ui_from_string(ui_info, ex);

	if(ex.get())
		cerr << "building menus failed: " <<  ex->what();
#endif
	Gtk::Widget* pMenubar = _refUIManager->get_widget("/MenuBar");
	if(pMenubar)
	    _Box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

	Gtk::Widget* pToolbar = _refUIManager->get_widget("/ToolBar");
	if(pToolbar)
   	_Box.pack_start(*pToolbar, Gtk::PACK_SHRINK);

	add(_Box);
	_Notebook.set_tab_pos(Gtk::POS_BOTTOM);
	_VPaned.add1(_Notebook);

	_VPaned.add2(_HPaned);
	_HPaned.add1(_MessageText);
	_HPaned.add2(_MessageText3);

	_Box.pack_start(_VPaned);
	_Box.pack_start(_HPaned);
	_Box.pack_start(_statusbar, Gtk::PACK_SHRINK);

	show_all_children();
	init_window_state();
	signal_configure_event().connect_notify(sigc::mem_fun(*this,
		&ExampleWindow::on_signal_resize));

	_HPaned.signal_button_release_event().connect_notify(sigc::mem_fun(*this,
		&ExampleWindow::on_paned_event));
	_VPaned.signal_button_release_event().connect_notify(sigc::mem_fun(*this,
		&ExampleWindow::on_paned_event));

	_Notebook.signal_page_removed().connect_notify(sigc::mem_fun(*this,
		&ExampleWindow::on_pages_changed));
	_Notebook.signal_page_added().connect_notify(sigc::mem_fun(*this,
		&ExampleWindow::on_pages_changed));

	_Notebook.signal_switch_page().connect_notify(sigc::mem_fun(*this,
		&ExampleWindow::on_page_focus));

	read_mru();
	vector<Gtk::TargetEntry> listTargets;
	listTargets.push_back(Gtk::TargetEntry("text/uri-list"));

	drag_dest_set(listTargets);
	signal_drag_data_received().connect(sigc::mem_fun(*this, &ExampleWindow::on_dropped_file));
}

ExampleWindow::~ExampleWindow()
{
	write_mru();
	write_window_geometry();

	if(_app->tools) {
		delete _app->tools;
		_app->tools = NULL;
	}
	if(_app->props) {
		delete _app->props;
		_app->props = NULL;
	}
	if(!_app->prj.empty()) {
		vector<project::HEXFILE*>::iterator it;
		for(it = _app->prj.begin(); it != _app->prj.end(); ++it)
			delete *it;
	}
}

void
ExampleWindow::write_mru()
{
	try {
		ofstream pfile(_app->path + "/mru.lst", ios::out | ios::trunc);
		if(pfile.is_open()) {
			deque<Glib::ustring>::iterator it = _app->mru.begin();
			for(; it != _app->mru.end(); ++it) {
				Glib::ustring file = (*it);
				if(file.length() > 1)
					pfile << file.c_str() << '\n';
			}
			pfile.flush();
			pfile.close();
		}
	}
	catch(exception& e) {
		Gtk::MessageDialog dialog(*this, "Hexlab");
		dialog.set_secondary_text("There was a problem writing the mru file.");
	}
	on_pages_mru();
}

void
ExampleWindow::read_mru()
{
	string str;
	try {
		ifstream pfile(_app->path + "/mru.lst");
		if(pfile.is_open()) {
			while(getline(pfile, str)) {
				if(str.length() > 1) {
					_app->mru.push_back(static_cast<Glib::ustring>(str));
					str.clear();
				}
			}
		}
		pfile.close();
		mru_from_serial();
	}
	catch(exception& e) {
		Gtk::MessageDialog dialog(*this, "Hexlab");
		dialog.set_secondary_text("There was a problem reading the mru file.");
	}
	on_pages_mru();
}

void
ExampleWindow::init_window_state()
{
	const Glib::ustring path = "/ui/MenuBar/FileMenu/FileSaveAs";
	const Glib::ustring mru_separator = "mru_sep";

	if(_app) {
		set_gravity(Gdk::GRAVITY_CENTER);
		Gtk::Widget* pToolbar = _refUIManager->get_widget("/ToolBar");

		Gtk::Widget* fileClose = _refUIManager->get_widget("/ui/MenuBar/FileMenu/FileClose");
		Gtk::Widget* fileSave = _refUIManager->get_widget("/ui/MenuBar/FileMenu/FileSave");
		Gtk::Widget* fileSaveAs = _refUIManager->get_widget(path);

		Gtk::Widget* file1 = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file1");
		Gtk::Widget* file2 = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file2");
		Gtk::Widget* file3 = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file3");
		Gtk::Widget* file4 = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file4");

		fileClose->hide();
		fileSave->hide();
		fileSaveAs->hide();

		file1->hide();
		file2->hide();
		file3->hide();
		file4->hide();

		(_app->tools->nToolbar)? pToolbar->show() : pToolbar->hide();
		dynamic_cast<Gtk::Toolbar*>(pToolbar)->set_toolbar_style(Gtk::TOOLBAR_ICONS);

		(_app->tools->nShowInspector)? _MessageText.show() : _MessageText.hide();
		(_app->tools->nShowResults)? _MessageText3.show() : _MessageText3.hide();

		resize(_app->tools->winW, _app->tools->winH);
		_HPaned.set_position(_app->tools->nHPane);
		_VPaned.set_position(_app->tools->nVPane);
	}
	else
		g_print("Report serious error\n");
}

void
ExampleWindow::on_menu_file_quit()
{
	Gtk::Main::quit();
}

void
ExampleWindow::on_menu_file_new_generic()
{
	std::cout << "A File|New menu item was selected." << std::endl;
}

void
ExampleWindow::on_menu_others()
{
	std::cout << "An menu item was selected." << std::endl;
}

void
ExampleWindow::on_menu_choices_one()
{
	Glib::ustring message;
	if(_refChoiceOne->get_active())
		message = "Choice 1 was selected.";
	else
		message = "Choice 1 was deselected";

  std::cout << message << std::endl;
}

void
ExampleWindow::on_menu_choices_two()
{
	Glib::ustring message;

	if(_refChoiceTwo->get_active())
		message = "Choice 2 was selected.";
	else
		message = "Choice 2 was deselected";

	std::cout << message << std::endl;
}

void
ExampleWindow::setup_statusbar_tooltip(
	const Glib::RefPtr<Gtk::Action>& action, Gtk::Widget* widget)
{
	if(dynamic_cast<Gtk::Widget*>(widget)) {
		(static_cast<Gtk::MenuItem*>(widget))->signal_select().connect(
			sigc::bind(sigc::mem_fun(*this, &ExampleWindow::menuitem_focus_cb), action));

		(static_cast<Gtk::MenuItem*>(widget))->signal_deselect().connect(
			sigc::mem_fun(*this, &ExampleWindow::on_clear_menu_tip));
	}
}

void
ExampleWindow::on_clear_menu_tip()
{
	_statusbar.pop();
}

void
ExampleWindow::menuitem_focus_cb(
	const Glib::RefPtr<Gtk::Action> &action)
{
	_statusbar.push(action->property_tooltip(),
		STATUSBAR_MENU_TIP);
}

void
ExampleWindow::on_app_about()
{
	if(_pAbout && _app->tools->bAboutShown) {
		_pAbout->set_transient_for(*this);
		Glib::RefPtr<Gdk::Window> about_win = _pAbout->get_window();
		about_win->show();
		about_win->raise();
	}
	else {
		if(_pAbout) {
			delete _pAbout;
			_pAbout = nullptr;
		}

		Gtk::AboutDialog* pDerived = new Gtk::AboutDialog;
		_pAbout = pDerived;
		pDerived->set_transient_for(*this);

		pDerived->set_program_name(APP_NAME);
		pDerived->set_version(APP_VER);
		pDerived->set_logo(Gdk::Pixbuf::create_from_file(_app->res_path +
		    "/res/labicn.png"));

		pDerived->set_copyright("Copyright © 2013-2020 Tim O'Neil");
		pDerived->set_comments("Based on Hex Workshop by BreakPoint Software");
		pDerived->set_comments("Just an attempt to make a full-featured GTKMM application");
		pDerived->set_license("LGPL");

		pDerived->set_website("http://www.bpsoft.com/");
		pDerived->set_website_label("BreakPoint Software Website");
		if(_vAuth.empty())
			_vAuth.push_back("Tim O'Neil <interval1066@gmail.com>");

		pDerived->set_authors(_vAuth);
		pDerived->set_documenters(_vAuth);
		_pAbout->signal_hide().connect(sigc::mem_fun(*this,
		    &ExampleWindow::on_about_close));

		pDerived->show_all_children();
		static_cast<Gtk::Dialog*>(_pAbout)->run();
		_app->tools->bAboutShown = true;

		_pAbout->hide();
	}
}

void
ExampleWindow::about_uri_cb(Gtk::AboutDialog &about,
	const Glib::ustring &link)
{
	GError* gerror = 0;
	gtk_show_uri_on_window(nullptr, "http://www.bpsoft.com/",
		GDK_CURRENT_TIME, &gerror);
}

void
ExampleWindow::on_about_close()
{
	_app->tools->bAboutShown = false;
}

void
ExampleWindow::on_toggle_toolbar()
{
	Gtk::Widget* pToolbar = _refUIManager->get_widget("/ToolBar");

	if(pToolbar->get_visible()) {
		pToolbar->hide();
		_app->tools->nToolbar = 0;
	}
	else {
		pToolbar->show();
		_app->tools->nToolbar = 1;
	}
}

void
ExampleWindow::on_toggle_datains()
{
	if(_MessageText.get_visible()) {
		_MessageText.hide();
		_app->tools->nShowInspector = 0;
	}
	else {
		_MessageText.show();
		_app->tools->nShowInspector = 1;
	}
	hide_hpane();
}

void
ExampleWindow::on_toggle_results()
{
	if(_MessageText3.get_visible()) {
		_MessageText3.hide();
		_app->tools->nShowResults = 0;
	}
	else {
		_MessageText3.show();
		_app->tools->nShowResults = 1;
	}
	hide_hpane();
}

void
ExampleWindow::on_pages_mru()
{
	Gtk::Widget* menuitem = _refUIManager->get_widget("/ui/MenuBar/OptionsMenu/clrmru");
	if(_app->mru.empty())
		menuitem->hide();
	else
		menuitem->show();
}

void
ExampleWindow::on_signal_resize(GdkEventConfigure* e)
{
	int w, h;
	get_size(w, h);

	_app->tools->winW = w;
	_app->tools->winH = h;
}

void
ExampleWindow::on_paned_event(GdkEventButton* e)
{
	_app->tools->nHPane = _HPaned.get_position();
	_app->tools->nVPane = _VPaned.get_position();
}

void
ExampleWindow::hide_hpane()
{
	 if(!_MessageText3.get_visible() && !_MessageText.get_visible())
		  _HPaned.hide();
	 else if(_MessageText3.get_visible() || _MessageText.get_visible())
		  _HPaned.show();
}

void
ExampleWindow::add_iconset()
{
	Glib::RefPtr<Gtk::IconFactory> icons =
		Gtk::IconFactory::create();
	icons->add_default();

	/*Glib::ustring icn_path = _app->res_path;
	icons->add(Gtk::StockID("FLIP"),
		Gtk::IconSet(Gdk::Pixbuf::create_from_file(icn_path.append("/res/flip.png").c_str())));

	icn_path = _app->res_path;
	icons->add(Gtk::StockID("INV"),
		Gtk::IconSet(Gdk::Pixbuf::create_from_file(icn_path.append("/res/inv.png").c_str())));

	icn_path = _app->res_path;
	icons->add(Gtk::StockID("CHK"),
		Gtk::IconSet(Gdk::Pixbuf::create_from_file(icn_path.append("/res/Check-icon.png").c_str())));

	icn_path = _app->res_path;
	icons->add(Gtk::StockID("BOOK"),
		Gtk::IconSet(Gdk::Pixbuf::create_from_file(icn_path.append("/res/bookmark.png").c_str())));*/
}

void
ExampleWindow::write_window_geometry()
{
 	_app->props->Set("windowW", to_string(_app->tools->winW));
	_app->props->Set("windowH", to_string(_app->tools->winH));

	_app->props->Set("inspect", to_string(_app->tools->nShowInspector));
	_app->props->Set("results", to_string(_app->tools->nShowResults));
	_app->props->Set("toolbar", to_string(_app->tools->nToolbar));

	_app->props->Set("hpane", to_string(_app->tools->nHPane));
	_app->props->Set("vpane", to_string(_app->tools->nVPane));
	_app->props->Write(_app->app_file.c_str());
}

void
ExampleWindow::get_window_geometry()
{
	_app->tools->winW = static_cast<int>(_app->props->Get("windowW", "1450"));
	_app->tools->winH = static_cast<int>(_app->props->Get("windowH", "500"));

	_app->tools->nShowInspector = static_cast<int>(_app->props->Get("inspect", "1"));
	_app->tools->nShowResults = static_cast<int>(_app->props->Get("results", "1"));
	_app->tools->nToolbar = static_cast<int>(_app->props->Get("toolbar", "1"));

	_app->tools->nHPane = static_cast<int>(_app->props->Get("hpane", "50"));
	_app->tools->nVPane = static_cast<int>(_app->props->Get("vpane", "50"));
}

int
ExampleWindow::create_props_file()
{
    if(mkdir(_app->path.c_str(), 0766) == 0) {
        ofstream pfile(_app->app_file.c_str());
        pfile << "# Hexlab properties file\n";
        pfile << "########################\n";

        pfile << "toolbar=1\n";
        pfile << "inspect=1\n";
        pfile << "results=1\n";

        pfile << "windowW=450\n";
        pfile << "windowH=400\n";
        pfile << "hpane=416\n";

        pfile << "vpane=410\n";
        pfile.close();
        _app->props = new Properties(_app->app_file.c_str(),
            MODE::PRP_READWRITE);

        return 0;
    }
    return -1;
}

void
ExampleWindow::on_file_open()
{
	Gtk::Button ok_Button;
	Gtk::FileChooserDialog dialog("Please choose a file",
		Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_APPLY);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();

	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog.add_filter(filter_any);

	dialog.set_select_multiple(true);
	if(dialog.run() == Gtk::RESPONSE_APPLY) {
		vector<string> files(dialog.get_filenames());

		for(vector<string>::iterator it = files.begin();
			it != files.end(); ++it) {
			HEXFILE* p = new HEXFILE;

			p->filepath = *it;
			_app->prj.push_back(p);
			add_file(p->filepath.c_str());
		}
	}
	on_pages_mru();
}

void
ExampleWindow::add_file(const Glib::ustring& text)
{
	Glib::ustring sLab = text.substr(text.find_last_of("/") + 1, text.length());
	if(!show_page(sLab)) {
		Glib::RefPtr<Gtk::TextBuffer> tbuf = Gtk::TextBuffer::create();

		Glib::RefPtr<Gtk::TextBuffer> tbuf2 = Gtk::TextBuffer::create();
		Glib::RefPtr<Gtk::TextBuffer> tbuf3 = Gtk::TextBuffer::create();
		Gtk::HPaned* hpaned = Gtk::manage(new Gtk::HPaned);

		Gtk::HPaned* hpaned2 = Gtk::manage(new Gtk::HPaned);
  		Gtk::ScrolledWindow* scwin = Gtk::manage(new Gtk::ScrolledWindow);
		Gtk::TextView* textview = Gtk::manage(new Gtk::TextView);

		Gtk::TextView* textview2 = Gtk::manage(new Gtk::TextView);
		ui::HexTextView* hexview = Gtk::manage(new ui::HexTextView);
		hpaned->add(*textview);

		textview->override_font(Pango::FontDescription("Monospace Bold 12"));
		textview->set_editable(false);
		textview->set_cursor_visible(false);

		textview->override_background_color(Gdk::RGBA("lightgray"), Gtk::STATE_FLAG_NORMAL);
		hpaned2->add(*hexview);
		hexview->override_font(Pango::FontDescription("Monospace Bold 12"));

		hexview->set_overwrite(true);
		hexview->set_editable(true);
		hexview->override_background_color(Gdk::RGBA("rgba(255, 255, 255, .5)"));

		hpaned2->add(*textview2);
		textview2->override_font(Pango::FontDescription("Monospace Bold 12"));
		hpaned->add(*hpaned2);

		tbuf->set_text("This is the text from TextBuffer #1.");
		textview->set_buffer(tbuf);
		tbuf2->set_text("This is the text from TextBuffer #1.");

		hexview->set_buffer(tbuf2);
		tbuf3->set_text("This is the text from TextBuffer #1.");
		textview2->set_buffer(tbuf3);

  		scwin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  		scwin->set_border_width(3);
  		scwin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  		scwin->add(*hpaned);
		_Notebook.append_page(*scwin, sLab);
		_Notebook.show_all();

		_Notebook.set_current_page(-1);
		manage_mru(text);
		show_page(sLab);
	}
	on_pages_mru();
}

void
ExampleWindow::on_menu_close()
{
	if(_Notebook.get_n_pages() > 0) {
		string sLab = _Notebook.get_tab_label_text(*(_Notebook.get_nth_page(_Notebook.get_current_page())));

		(_Notebook.get_n_pages() == 1)? _Notebook.remove_page(-1) :
			_Notebook.remove_page(_Notebook.get_current_page());

		vector<project::HEXFILE*>::iterator it;
		for(it = _app->prj.begin(); it != _app->prj.end(); ++it) {
			if(sLab.compare((*it)->filename) == 0)
				delete *it;
		}
	}
}

void
ExampleWindow::on_pages_changed(Gtk::Widget* page, guint page_num)
{
	Gtk::Widget* fileClose =
		_refUIManager->get_widget("/ui/MenuBar/FileMenu/FileClose");
	Gtk::Widget* fileSave =
		_refUIManager->get_widget("/ui/MenuBar/FileMenu/FileSave");
	Gtk::Widget* fileSaveAs =
		_refUIManager->get_widget("/ui/MenuBar/FileMenu/FileSaveAs");

	if(_Notebook.get_n_pages() < 1) {
		fileClose->hide();
		fileSave->hide();
		fileSaveAs->hide();
	}
	else {
		fileClose->show();
		fileSave->show();
		fileSaveAs->show();
	}
	on_pages_mru();
}

void
ExampleWindow::ErrorDlg(const Glib::ustring& msg)
{
    Gtk::MessageDialog dlg(msg, false,
		Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    dlg.set_title(APP_NAME);

    dlg.run();
}

void
ExampleWindow::on_menu_options()
{
	_pOptions = new OptionsDlg(_app->props);
}

/**
 * manage_mru
 *
 * Deceptively small method but there's a lot going on
 * here; first- keeps track of the mru list by popping
 * and pushing recent files on a deque.
 *
 * Then we connect signals to those menu items. You must
 * disconnect any existing signals first or you'll have
 * multiple signals emitted per menu item. Apparently
 * disconnecting non-existing signals is not an issue,
 * so we make sure there are non regardless before
 * connecting new ones. We keep track of signals using
 * a simple array of type "sigc::connection" that's a
 * member of our big "_app" struct.

 * Look at the glib ui string under the file menu
 * to see the place holders. We simply hide/show &
 * change those labels to simluate an MRU becuase the
 * one that comes with Gtk/Gtkmm is too heavyweight
 * and too weird.

 */
void
ExampleWindow::manage_mru(const Glib::ustring& text)
{
	bool bFound = false;
	Gtk::Widget* file[4];

	file[0] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file1");
	file[1] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file2");
	file[2] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file3");
	file[3] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file4");

	deque<Glib::ustring>::iterator it = _app->mru.begin();
	for(; it != _app->mru.end(); ++it)
		if((*it).find(text) != string::npos) bFound = true;

	if(!bFound) {
		_app->mru.push_front(text);
		if(_app->mru.size() >= 4)
			_app->mru.pop_back();
		_app->mru.resize(4);

		int n = 0;
		for(deque<Glib::ustring>::iterator it =
			_app->mru.begin(); it < _app->mru.end(); ++it) {

			const Glib::ustring& label =
				(*it).substr((*it).find_last_of("//") + 1, (*it).length());

			dynamic_cast<Gtk::MenuItem*>(file[n])->set_label(label.c_str());
			_app->mru_sig[n].disconnect();

			_app->mru_sig[n] = dynamic_cast<Gtk::MenuItem*>(file[n])->signal_activate().
				connect(sigc::bind(sigc::mem_fun(*this, &ExampleWindow::on_mru_list), label));

			if(dynamic_cast<Gtk::MenuItem*>(file[n])->get_label().length() > 0)
				file[n++]->show();
		}
	}
	on_pages_mru();
}

void
ExampleWindow::on_mru(const Glib::ustring& label)
{
	Glib::ustring msg;
	//ostringstream os;
	deque<Glib::ustring>::const_iterator it = _app->mru.begin();

	for(; it < _app->mru.end(); ++it) {
		const Glib::ustring& text =
			(*it).substr((*it).find_last_of("//") + 1, (*it).length());

		if(text.find(label) != string::npos)
			show_page(label);
		else {

			HEXFILE* p = new HEXFILE;
			p->filepath = label;
			_app->prj.push_back(p);

			add_file(p->filepath.c_str());
		}
	}
}

void
ExampleWindow::on_mru_list(Glib::ustring& label)
{
	if(!show_page(label)) {
		deque<Glib::ustring>::const_iterator it = _app->mru.begin();
		for(; it < _app->mru.end(); ++it) {

			const Glib::ustring& text =
				(*it).substr((*it).find_last_of("//") + 1, (*it).length());

			if(label.find(text) != Glib::ustring::npos)
				add_file((*it));
		}
	}
}

bool
ExampleWindow::show_page(const Glib::ustring& text)
{
	auto last_page = _Notebook.get_n_pages();
	for(auto i = 0; i != last_page; i++) {

		const Glib::ustring str =
			_Notebook.get_tab_label_text(*(_Notebook.get_nth_page(i)));

		if(text.find(str) != Glib::ustring::npos) {
			_Notebook.set_current_page(i);
			set_tab(i);
			return true;
		}
	}
	return false;
}

// This is for getting the file name from the tab to display in the main frame.
void
ExampleWindow::on_page_focus(Gtk::Widget* page, guint page_num)
{
	set_tab(page_num);
}

void
ExampleWindow::set_tab(guint page_num)
{
	array<char, 256> buf;
	buf.fill('\0');
	Glib::ustring path2;

	const Glib::ustring str =
		_Notebook.get_tab_label_text(*(_Notebook.get_nth_page(page_num)));
	deque<Glib::ustring>::iterator it = _app->mru.begin();

	for(; it < _app->mru.end(); ++it) {
		size_t len = (*it).length();
		const Glib::ustring& text =
			(*it).substr((*it).find_last_of("//") + 1, len);

		if(str.find(text) != Glib::ustring::npos) {
			copy((*it).begin(), (*it).end(), buf.data());
			Glib::ustring path(buf.begin(), buf.end() - (256 - len));

			path2 = path;
			break;
		}
	}
	Glib::ustring title(path2.append(" - Hexlab"));
	this->set_title(title);
}

void
ExampleWindow::mru_from_serial()
{
	gint n = 0;
	Gtk::Widget* file[4];
	deque<Glib::ustring>::const_iterator it;

	file[0] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file1");
	file[1] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file2");
	file[2] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file3");
	file[3] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file4");

	for(it = _app->mru.begin(); it != _app->mru.end(); ++it) {
		const Glib::ustring& label =
			(*it).substr((*it).find_last_of("//") + 1, (*it).length());

		dynamic_cast<Gtk::MenuItem*>(file[n])->set_label(label.c_str());
		_app->mru_sig[n].disconnect();

		_app->mru_sig[n] = dynamic_cast<Gtk::MenuItem*>(file[n])->signal_activate().
			connect(sigc::bind(sigc::mem_fun(*this, &ExampleWindow::on_mru_list), label));

		if(dynamic_cast<Gtk::MenuItem*>(file[n])->get_label().length() > 0)
			file[n++]->show();
	}
	on_pages_mru();
}

void
ExampleWindow::on_dropped_file(const Glib::RefPtr<Gdk::DragContext>& context,
	int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time)
{
	if((selection_data.get_length() >= 0) &&
		(selection_data.get_format() == 8)) {
		vector<Glib::ustring> file_list;

 		file_list = selection_data.get_uris();
		if(file_list.size() > 0) {
			const Glib::ustring path = Glib::filename_from_uri(file_list[0]);
            if(nullptr != path) {
    			if(!is_directory(path))
	    			add_file(path);
            }
			context->drag_finish(true, false, time);
			return;
		}
	}
	context->drag_finish(false, false, time);
	on_pages_mru();
}

void
ExampleWindow::on_file_accel(const Glib::ustring& str)
{
	deque<Glib::ustring>::const_iterator it;
	Gtk::Widget* file = _refUIManager->get_widget(str.c_str());
	Glib::ustring item = dynamic_cast<Gtk::MenuItem*>(file)->get_label();

	for(it = _app->mru.begin(); it != _app->mru.end(); ++it) {
		const Glib::ustring& label =
			(*it).substr((*it).find_last_of("//") + 1, (*it).length());

		if(label.compare(item) == 0)
			add_file((*it));
	}
}

void
ExampleWindow::on_file1_accel()
{
	on_file_accel("/ui/MenuBar/FileMenu/file1");
}

void
ExampleWindow::on_file2_accel()
{
	on_file_accel("/ui/MenuBar/FileMenu/file2");
}

void
ExampleWindow::on_file3_accel()
{
	on_file_accel("/ui/MenuBar/FileMenu/file3");
}

void
ExampleWindow::on_file4_accel()
{
	on_file_accel("/ui/MenuBar/FileMenu/file4");
}

bool
ExampleWindow::is_directory(const Glib::ustring& str)
{
	DIR* file = opendir(str.c_str());
	if(nullptr != file) {
		closedir(file);
		return true;
	}
	return false;
}

void
ExampleWindow::ClearMRU()
{
	string mru_lst = _app->path + "/mru.lst";
	try {
		RefPtr<File> file = File::create_for_path(mru_lst);
		file->remove();
		file->create_file();
	}
	catch(Glib::Error& er) { }

	gint n = 0;
	Gtk::Widget* menu[4];
	deque<Glib::ustring>::const_iterator it;

	menu[0] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file1");
	menu[1] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file2");
	menu[2] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file3");
	menu[3] = _refUIManager->get_widget("/ui/MenuBar/FileMenu/file4");

	for(it = _app->mru.begin(); it != _app->mru.end(); ++it) {
		_app->mru_sig[n].disconnect();
		menu[n++]->hide();
	}
	_app->mru.clear();
	on_pages_mru();
}

