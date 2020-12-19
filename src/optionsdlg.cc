#include <iostream>
#include <optionsdlg.h>
#include <utils/properties.h>
#include <examplewindow.h>

using namespace std;

OptionsDlg::OptionsDlg(utils::Properties* prp)
{
	std::string buttonIdStr;
	_prp = prp;
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

	_refBuilder->get_widget("checkbutton2", pCheck);
	_refBuilder->get_widget("checkbutton3", pCheck2);
	_refBuilder->get_widget("checkbutton4", pCheck3);

	_refBuilder->get_widget("checkbutton5", pCheck4);
	_refBuilder->get_widget("checkbutton6", pCheck5);
	_refBuilder->get_widget("entry6", pBufsize);

	_refBuilder->get_widget("entry1", pFloat);
	_refBuilder->get_widget("entry2", pDouble);
	_refBuilder->get_widget("checkbutton8", pCheck6);

	_refBuilder->get_widget("checkbutton9", pCheck7);
	_refBuilder->get_widget("checkbutton10", pCheck8);
	_refBuilder->get_widget("checkbutton11", pCheck9);

	_refBuilder->get_widget("checkbutton12", pCheck10);
	_refBuilder->get_widget("checkbutton13", pCheck11);
	_refBuilder->get_widget("checkbutton14", pCheck12);

	_refBuilder->get_widget("checkbutton15", pCheck13);
	_refBuilder->get_widget("button4", pOpenFrom);
	_refBuilder->get_widget("button5", pImportFrom);

	_refBuilder->get_widget("button6", pTempFiles);
	_refBuilder->get_widget("entry3", pOpenFromPath);
	_refBuilder->get_widget("entry4", pImportFromPath);

	_refBuilder->get_widget("entry5", pTempPath);

	pCancel->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_cancel));
	pOK->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_OK));

	pCheck11->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_loadfrom_clicked));

	pImportFrom->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_select_importfrom));

	pOpenFrom->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_select_openfrom));

	pTempFiles->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_select_tempfiles));

	Init_Ctrls();
	pDialog->run();
}

OptionsDlg::~OptionsDlg()
{
}

void
OptionsDlg::Init_Ctrls()
{
	(static_cast<bool>(_prp->Get("backups", "0"))) ?
		pChkBakup->set_active(true) : pChkBakup->set_active(false);

	(static_cast<bool>(_prp->Get("confirm", "0"))) ?
		pCheck->set_active(true) : pCheck->set_active(false);

	(static_cast<bool>(_prp->Get("noto", "0"))) ?
		pCheck2->set_active(true) : pCheck2->set_active(false);

	(static_cast<bool>(_prp->Get("env", "0"))) ?
		pCheck3->set_active(true) : pCheck3->set_active(false);

	(static_cast<bool>(_prp->Get("max", "0"))) ?
		pCheck4->set_active(true) : pCheck4->set_active(false);

	(static_cast<bool>(_prp->Get("workbook", "0"))) ?
		pCheck5->set_active(true) : pCheck5->set_active(false);

    string plah = _prp->Get("bufsize", "128");
	pBufsize->set_text(plah);
	string plah2 = _prp->Get("float", "8");
	pFloat->set_text(plah2);
	string plah3 = _prp->Get("double", "8");
	pDouble->set_text(plah3);

	(static_cast<bool>(_prp->Get("addnaut", "0"))) ?
		pCheck6->set_active(true) : pCheck6->set_active(false);

	(static_cast<bool>(_prp->Get("addthun", "0"))) ?
		pCheck7->set_active(true) : pCheck7->set_active(false);

	(static_cast<bool>(_prp->Get("multiple", "0"))) ?
		pCheck8->set_active(true) : pCheck8->set_active(false);

	(static_cast<bool>(_prp->Get("direct", "0"))) ?
		pCheck9->set_active(true) : pCheck9->set_active(false);

	(static_cast<bool>(_prp->Get("postpone", "0"))) ?
		pCheck10->set_active(true) : pCheck10->set_active(false);

	(static_cast<bool>(_prp->Get("openfrom", "0"))) ?
		pCheck11->set_active(true) : pCheck11->set_active(false);

	(static_cast<bool>(_prp->Get("importfrom", "0"))) ?
		pCheck12->set_active(true) : pCheck12->set_active(false);

    string plah4 = _prp->Get("openpath", "");
	pOpenFromPath->set_text(plah4);
	string plah5 = _prp->Get("importfilespath", "");
	pImportFromPath->set_text(plah5);
	string plah6 = _prp->Get("tempfilespath", "");
	pTempPath->set_text(plah6);

}

void
OptionsDlg::on_OK()
{
	_prp->Set("backups", pChkBakup->get_active() ? "1" : "0");
	_prp->Set("confirm", pCheck->get_active() ? "1" : "0");
	_prp->Set("noto", pCheck2->get_active() ? "1" : "0");

	_prp->Set("env", pCheck3->get_active() ? "1" : "0");
	_prp->Set("max", pCheck4->get_active() ? "1" : "0");
	_prp->Set("workbook", pCheck5->get_active() ? "1" : "0");

	_prp->Set("bufsize", pBufsize->get_text());
	_prp->Set("float", pFloat->get_text());
	_prp->Set("double", pDouble->get_text());

	_prp->Set("addnaut", pCheck6->get_active() ? "1" : "0");
	_prp->Set("addthun", pCheck7->get_active() ? "1" : "0");
	_prp->Set("multiple", pCheck8->get_active() ? "1" : "0");

	_prp->Set("direct", pCheck9->get_active() ? "1" : "0");
	_prp->Set("postpone", pCheck10->get_active() ? "1" : "0");
	_prp->Set("openfrom", pCheck11->get_active() ? "1" : "0");

	_prp->Set("importfrom", pCheck12->get_active() ? "1" : "0");
	_prp->Set("tempfiles", pCheck13->get_active() ? "1" : "0");

	_prp->Set("openpath", pOpenFromPath->get_text());
	_prp->Set("importfilespath", pImportFromPath->get_text());
	_prp->Set("tempfilespath", pTempPath->get_text());

	pDialog->hide();
}

void
OptionsDlg::on_cancel()
{
	pDialog->hide();
}

void
OptionsDlg::on_loadfrom_clicked()
{
	Gtk::FileChooserDialog dialog("Please choose a folder",
		Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);

	int result = dialog.run();
	switch(result) {
		case(Gtk::RESPONSE_OK):
      	pOpenFromPath->set_text(dialog.get_filename().c_str());
			break;

		default:
			break;
    }
}

void
OptionsDlg::on_select_importfrom()
{
	Gtk::FileChooserDialog dialog("Please choose a folder",
		Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);

	int result = dialog.run();
	switch(result) {
		case(Gtk::RESPONSE_OK):
      	pImportFromPath->set_text(dialog.get_filename().c_str());
			break;

		default:
			break;
    }
}

void
OptionsDlg::on_select_openfrom()
{
	Gtk::FileChooserDialog dialog("Please choose a folder",
		Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);

	int result = dialog.run();
	switch(result) {
		case(Gtk::RESPONSE_OK):
      	pOpenFromPath->set_text(dialog.get_filename().c_str());
			break;

		default:
			break;
    }
}

void
OptionsDlg::on_select_tempfiles()
{
	Gtk::FileChooserDialog dialog("Please choose a folder",
		Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);

	int result = dialog.run();
	switch(result) {
		case(Gtk::RESPONSE_OK):
      	pTempPath->set_text(dialog.get_filename().c_str());
			break;

		default:
			break;
    }
}

