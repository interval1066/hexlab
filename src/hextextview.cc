#include "ui/hextextview.h"
#include "ui/hextextiter.h"
//#include "hexdump.h"
#include <iostream>

using namespace ui;

HexTextView::HexTextView() { }

HexTextView::~HexTextView() { }

bool
HexTextView::on_key_press_event(GdkEventKey* event)
{
	bool processed = false;

   if (GDK_KEY_PRESS == event->type) {
    switch (event->keyval) {

    	case GDK_KEY_Left:
      	move_to_next_hexnum(0);
     		break;

    	case GDK_KEY_Up:
      	std::cout << "Up: " << std::endl;
      	break;

    	case GDK_KEY_Down:
      	std::cout << "Down: " << std::endl;
      	break;

    	case GDK_KEY_Right:
      	move_to_next_hexnum(1);
      	break;
    	}
	}
	if(!processed)
		processed = Gtk::Widget::on_key_press_event(event);
	return processed;
}

void
HexTextView::move_to_next_hexnum(int dir)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = get_buffer();
	HexTextIter cursor = get_cursor_iter(buffer); 

	HexTextIter iter(cursor);
	for(int i = 0; i < 1; ++i) {
		if(dir == 0)
			iter.backward_next_word_start();
		else
			iter.forward_next_word_start();
		set_cursor(iter, true);
	}
}

HexTextIter
HexTextView::get_cursor_iter(Glib::RefPtr<Gtk::TextBuffer> buffer)
{
   Glib::RefPtr<Gtk::TextBuffer::Mark> cursor;

	cursor = buffer->get_insert();
	return HexTextIter(buffer->get_iter_at_mark(cursor));
}

void
HexTextView::set_cursor(Gtk::TextBuffer::iterator location, bool ext_sel)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = location.get_buffer();
	if(ext_sel) {
		Glib::RefPtr< Gtk::TextBuffer::Mark > sel_bound =
			buffer->get_selection_bound();
		buffer->move_mark(sel_bound, location);
	}
	else 
		buffer->place_cursor(location);
}

