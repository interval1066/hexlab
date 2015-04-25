#ifndef GTKWIDGET_FACTORY_H
#define GTKWIDGET_FACTORY_H

#include <gtkmm.h>

namespace ui
{
	class GtkWidgetFactory
	{
		static Gtk::Widget* make_widget(int);
	};
}

#endif

