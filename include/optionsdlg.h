#ifndef OPTIONS_H
#define OPTIONS_H

#include <gtkmm.h>
#include <sigc++/sigc++.h>

class OptionsDlg
{
public:
	OptionsDlg();
	virtual ~OptionsDlg();

protected:
	Glib::RefPtr<Gtk::Builder> _refBuilder;

	virtual void on_cancel(void);
	Gtk::Dialog* pDialog;
	Gtk::Button* pOK;

	Gtk::Button* pCancel;
	Gtk::CheckButton* pChkBakup;
	void on_OK(void);
};

#endif

