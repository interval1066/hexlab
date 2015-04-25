#ifndef OPTIONS_H
#define OPTIONS_H

#include <gtkmm.h>
#include <unordered_map>
#include <utils/properties.h>

class OptionsDlg
{
	std::string checks[13] = {
		"checkbutton1",
		"checkbutton2",
		"checkbutton3",
		"checkbutton4",
		"checkbutton5",
		"checkbutton6",
		"checkbutton7",
		"checkbutton8",
		"checkbutton9",
		"checkbutton10",
		"checkbutton11",
		"checkbutton12",
		"checkbutton13" };

	std::string props[16] = {
		"backups",
		"confirm",
		"noto",
		"env",
		"max",
		"workbook",
		"addnaut",
		"addthun",
		"multiple",
		"direct",
		"postpone",
		"openfrom",
		"importfrom",
		"openpath",
		"importfilespath",
		"tempfilespath" };

	std::unordered_map<std::string, Gtk::CheckButton*> checkmap;
	
protected:
	Glib::RefPtr<Gtk::Builder> _refBuilder;
	Glib::RefPtr<Gtk::ToggleAction> _togBackup;
	void on_OK(void);

	virtual void on_cancel(void);
	Gtk::Dialog* pDialog;
	Gtk::Button* pOK;

	Gtk::Button* pCancel, *pImportFrom, * pOpenFrom, * pTempFiles;
	Gtk::CheckButton* pChkBakup, * pCheck, * pCheck2, * pCheck3, * pCheck4,
		* pCheck5, * pCheck6, * pCheck7, * pCheck8, * pCheck9, * pCheck10,
		* pCheck11, * pCheck12, * pCheck13;
	Gtk::CheckButton * pCheckLoad[3];

	Gtk::Entry* pBufsize, * pFloat, * pDouble, * pOpenFromPath,
		* pImportFromPath, * pTempPath;
	void Init_Ctrls(void);
	void on_loadfrom_clicked(void);

	void on_select_importfrom(void);
	void on_select_openfrom(void);
	void on_select_tempfiles(void);

public:
	OptionsDlg(utils::Properties*);
	virtual ~OptionsDlg();
	utils::Properties* _prp;
};

#endif

