#ifndef HEXTEXTVIEW_H_
#define HEXTEXTVIEW_H_

#include <gtkmm.h>
#include <glib.h>
#include "ui/hextextiter.h"

namespace ui
{
	class HexTextView : public Gtk::TextView
	{
		void move_to_next_hexnum(int);

	public:
		HexTextView();
		virtual ~HexTextView();
		virtual bool on_key_press_event(GdkEventKey*);

		ui::HexTextIter get_cursor_iter(Glib::RefPtr<Gtk::TextBuffer>);
		void set_cursor(Gtk::TextBuffer::iterator, bool);
	};
}

#endif

