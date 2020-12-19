#ifndef HEX_STATUSBAR_
#define HEX_STATUSBAR_

#include <gtkmm/statusbar.h>

namespace ui
{
	class HexlabStatusbar : public Gtk::Statusbar
	{
	public:
		HexlabStatusbar();
		virtual ~HexlabStatusbar();
	};
}

#endif

