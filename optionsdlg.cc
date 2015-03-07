#include <iostream>
#include <optionsdlg.h>
#include <examplewindow.h>

using namespace std;

OptionsDlg::OptionsDlg()
{
	ExampleWindow parent;
	_refBuilder = Gtk::Builder::create();
	pDialog = dynamic_cast<Gtk::Dialog*>(this);
	try {
		_refBuilder->add_from_file("/usr/share/hexlab/dlgs/Options.glade");
	}
	catch(const Glib::FileError& ex) {
		cerr << "FileError: " << ex.what() << endl;
	}
	_refBuilder->get_widget("dialog1", pDialog);
	pDialog->set_title("Preferences");
	pDialog->set_resizable(false);

	pDialog->set_transient_for(parent);
	_refBuilder->get_widget("ok", pOK);
	_refBuilder->get_widget("cancel", pCancel);
	_refBuilder->get_widget("checkbutton1", pChkBakup);

	pCancel->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_cancel));
	pOK->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_OK));

	pDialog->run();
}

OptionsDlg::~OptionsDlg()
{
}

void
OptionsDlg::on_OK()
{
	//if(
}

void
OptionsDlg::on_cancel()
{
	pDialog->hide();
}

