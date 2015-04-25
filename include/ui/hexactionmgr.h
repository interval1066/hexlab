#ifndef HEXACTIONMGR_H
#define HEXACTIONMGR_H

#include <ui/actionmgr.h>
#include <unordered_map>
#include <bitset>
#include <string>
#include <utility>

namespace ui
{
	class HexActionMgr : public ActionMgrBase
	{
		std::unordered_map<Glib::RefPtr<Gtk::UIManager>, std::string> _uimgr;
		std::unordered_map<Glib::RefPtr<Gtk::ActionGroup>, std::string> _uigroup;
		std::unordered_map<Glib::RefPtr<Gtk::IconFactory>, std::string> _uiiconfactory;

	public:
		HexActionMgr() {}
		virtual ~HexActionMgr() {}
		Glib::RefPtr<Gtk::UIManager> CreateUIManager();
		Glib::RefPtr<Gtk::ActionGroup> CreateActionGrp();
		Glib::RefPtr<Gtk::IconFactory> CreateIconFactory();

		void AddUIMgr2Q(Glib::RefPtr<Gtk::UIManager>, std::string);
		void AddActionGroup2Q(Glib::RefPtr<Gtk::ActionGroup>, std::string);
		void AddIconFactory2Q(Glib::RefPtr<Gtk::IconFactory>, std::string);

		Glib::RefPtr<Gtk::UIManager> FindUIMgr(std::string);
		Glib::RefPtr<Gtk::ActionGroup> FindUIActionGrp(std::string);
		Glib::RefPtr<Gtk::IconFactory> FindUIIconFactory(std::string);
}

#endif

