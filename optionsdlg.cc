#include <iostream>
#include <optionsdlg.h>
#include <utils/properties.h>
#include <examplewindow.h>

using namespace std;

OptionsDlg::OptionsDlg(utils::Properties* prp)
{
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
	pCancel->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_cancel));
	pOK->signal_clicked().connect(sigc::mem_fun(*this,
		&OptionsDlg::on_OK));

	Init_Ctrls();
	pDialog->run();
}

OptionsDlg::~OptionsDlg()
{
}

void
OptionsDlg::Init_Ctrls()
{
	(_prp->Get("backups", "0").find("1") != string::npos) ?
		pChkBakup->set_active(true) : pChkBakup->set_active(false);

	(_prp->Get("confirm", "0").find("1") != string::npos) ?
		pCheck->set_active(true) : pCheck->set_active(false);

	(_prp->Get("noto", "0").find("1") != string::npos) ?
		pCheck2->set_active(true) : pCheck2->set_active(false);

	(_prp->Get("env", "0").find("1") != string::npos) ?
		pCheck3->set_active(true) : pCheck3->set_active(false);

	(_prp->Get("max", "0").find("1") != string::npos) ?
		pCheck4->set_active(true) : pCheck4->set_active(false);

	(_prp->Get("workbook", "0").find("1") != string::npos) ?
		pCheck5->set_active(true) : pCheck5->set_active(false);

	pBufsize->set_text(_prp->Get("bufsize", "128").c_str());
	pFloat->set_text(_prp->Get("float", "8").c_str());
	pDouble->set_text(_prp->Get("double", "8").c_str());

	(_prp->Get("add_naut", "0").find("1") != string::npos) ?
		pCheck6->set_active(true) : pCheck6->set_active(false);

	(_prp->Get("add_thun", "0").find("1") != string::npos) ?
		pCheck7->set_active(true) : pCheck7->set_active(false);

	(_prp->Get("multiple", "0").find("1") != string::npos) ?
		pCheck8->set_active(true) : pCheck8->set_active(false);

	(_prp->Get("direct", "0").find("1") != string::npos) ?
		pCheck9->set_active(true) : pCheck9->set_active(false);

	(_prp->Get("postpone", "0").find("1") != string::npos) ?
		pCheck10->set_active(true) : pCheck10->set_active(false);
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

	_prp->Set("add_naut", pCheck6->get_active() ? "1" : "0");
	_prp->Set("add_thun", pCheck7->get_active() ? "1" : "0");
	_prp->Set("multiple", pCheck8->get_active() ? "1" : "0");

	_prp->Set("direct", pCheck9->get_active() ? "1" : "0");
	_prp->Set("postpone", pCheck10->get_active() ? "1" : "0");
	pDialog->hide();
}

void
OptionsDlg::on_cancel()
{
	pDialog->hide();
}

