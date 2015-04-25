#ifndef ACTIONMGRBASE_H
#define ACTIONMGRBASE_H

namespace ui
{
	class ActionMgrBase
	{
	public:
		virtual Glib::RefPtr<Gtk::UIManager> CreateUIManager();
		virtual Glib::RefPtr<Gtk::ActionGroup> CreateActionGrp();
		virtual Glib::RefPtr<Gtk::IconFactory> CreateIconFactory();
	};
}

#endif

