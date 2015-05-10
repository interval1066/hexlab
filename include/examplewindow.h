//$Id: examplewindow.h 705 20012-08-19 12:48:02Z to $ -*- c++ -*-
/**
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>
#include "utils/properties.h"
#include "messageslist.h"
#include "messagetext2.h"
#include "messagetext3.h"
#include <optionsdlg.h>
#include <defines.h>
#include <project/project.h>
#include <deque>
#include <sigc++/sigc++.h>

#define BUFFSIZE		2048

/**
 * Helper template for any linear (non-pair, for example) container
 * Usage:
 * std::deque<std::string> d;
 * stl::string str = giveme(d, "thestring"));
 */
template<typename T, class E>
T giveme(const T& pile, const E& thing)
{
	typename T::const_iterator F = pile.begin(), L = pile.end();
	for (; F != L; ++F)
		if(*F == thing) return thing;

	return NULL;
}

class ExampleWindow : public Gtk::Window
{
	typedef struct
	{
		bool	bAboutShown;
		int	nToolbar;
		int	nShowResults;
		int	nShowInspector;
		int	nHPane;
		int	nVPane;
		int	winW;
		int	winH;
	} DETAIL_WINDOWS;

	typedef struct
	{
		Glib::ustring							path;
		Glib::ustring							res_path;
		Glib::ustring							app_file;
		Glib::ustring							last_path;
		DETAIL_WINDOWS*						tools;
		utils::Properties*					props;
		std::vector<project::HEXFILE*>	prj;
		std::deque<Glib::ustring>			mru;
		sigc::connection						mru_sig[4];
	} APP_DETAIL;

	APP_DETAIL* _app;
	Gtk::Image m_image;
	virtual void hide_hpane();
	
	void add_iconset(void);
	void write_window_geometry(void);
	void create_props_file(void);

	void get_window_geometry(void);
	virtual void init_window_state(void);
	void set_tab(guint);

	void write_mru(void);
	void read_mru(void);
	void mru_from_serial(void);

	void on_file_accel(const Glib::ustring&);
	void on_file1_accel(void);
	void on_file2_accel(void);

	void on_file3_accel(void);
	void on_file4_accel(void);
	void on_dropped_file(const Glib::RefPtr<Gdk::DragContext>&,
		int, int, const Gtk::SelectionData&, guint, guint);

	bool is_directory(const Glib::ustring&);
	void on_clear_menu_tip(void);

public:
	ExampleWindow();
	virtual ~ExampleWindow();
	void ClearMRU(void);

	typedef sigc::signal<void> type_signal_mru;
	type_signal_mru signal_mru();

	enum
	{
		STATUSBAR_DEFAULT,
		STATUSBAR_MENU_TIP,
		STATUSBAR_TREESELECTION
	};

protected:
	Gtk::MenuItem* _fileClose;
	static Gtk::Window* _pAbout;
	OptionsDlg* _pOptions;

	Glib::RefPtr<Gtk::UIManager> _refUIManager;
	Glib::RefPtr<Gtk::ActionGroup> _refActionGroup;
	Glib::RefPtr<Gtk::ToggleAction> _refChoiceOne, _refChoiceTwo;

	Glib::RefPtr<Gtk::ToggleAction> _refOffsetHex, _refOffsetDec;
	Glib::RefPtr<Gtk::Builder> _refBuilder;

	void about_uri_cb(Gtk::AboutDialog& dialog,
		const Glib::ustring& link);

	Gtk::Statusbar _statusbar, _statusbar2, _statusbar3, _statusbar4;
	Gtk::Fixed _fixed;
	Gtk::AspectFrame _frame;
	Gtk::Alignment _align, _align2;
	Gtk::VPaned _VPaned;
	Gtk::HPaned _HPaned;
	 
	Gtk::Notebook _Notebook;
  	MessagesList _MessagesList;
  	MessageText2 _MessageText;
	 
	MessageText3 _MessageText3;
	Gtk::Label _Label1, _Label2;
	Gtk::VBox _Box, _space;

	Gtk::AspectFrame _aspect_frame;
	Gtk::HBox _SBox;

	virtual void on_menu_file_new_generic(void);
	virtual void on_menu_file_quit(void);
	virtual void on_menu_close(void);

	virtual void on_pages_changed(Gtk::Widget*, guint);
	virtual void on_menu_others(void);
	virtual void on_pages_mru(void);

	virtual void on_menu_choices_one(void);
	virtual void on_menu_choices_two(void);
	virtual void on_app_about(void);

	virtual void on_about_close(void);	
	virtual void on_toggle_toolbar(void);
	virtual void on_toggle_datains(void);
	 
	virtual void on_toggle_results(void);
	virtual void on_signal_resize(GdkEventConfigure*);
	virtual void on_paned_event(GdkEventButton*);

	virtual void on_menu_options(void);
	virtual void menuitem_focus_cb(const Glib::RefPtr<Gtk::Action>& action);
	std::vector<Glib::ustring> _vAuth;

	virtual void setup_statusbar_tooltip(const Glib::RefPtr<Gtk::Action>& action,
		Gtk::Widget* widget);

	virtual void on_file_open(void);
	virtual void add_file(const Glib::ustring&);
	virtual void ErrorDlg(const Glib::ustring&);

	virtual void manage_mru(const Glib::ustring&);
	virtual void on_mru(const Glib::ustring&);
	virtual void on_page_focus(Gtk::Widget*, guint);

	virtual bool show_page(const Glib::ustring&);
	virtual void on_mru_list(Glib::ustring&);
};

#endif

