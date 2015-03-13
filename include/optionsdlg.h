#ifndef OPTIONS_H
#define OPTIONS_H

#include <gtkmm.h>
#include <utils/properties.h>

class OptionsDlg
{
protected:
	Glib::RefPtr<Gtk::Builder> _refBuilder;
	Glib::RefPtr<Gtk::ToggleAction> _togBackup;
	void on_OK(void);

	virtual void on_cancel(void);
	Gtk::Dialog* pDialog;
	Gtk::Button* pOK;

	Gtk::Button* pCancel;
	Gtk::CheckButton* pChkBakup, * pCheck, * pCheck2, * pCheck3, * pCheck4,
		* pCheck5, * pCheck6, * pCheck7, * pCheck8, * pCheck9, * pCheck10;

	Gtk::Entry* pBufsize, * pFloat, * pDouble;
	void Init_Ctrls(void);

public:
	OptionsDlg(utils::Properties*);
	virtual ~OptionsDlg();
	utils::Properties* _prp;
};

#endif

