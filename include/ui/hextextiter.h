#ifndef HEXTEXTITER_H_
#define HEXTEXTITER_H_

#include <gtkmm.h>

namespace ui
{
	class HexTextIter : public Gtk::TextIter
	{
	public:
		HexTextIter(const Gtk::TextIter& iter);
		HexTextIter(const HexTextIter& iter);
		virtual ~HexTextIter();

		bool iter_next(GtkDirectionType dir);
		bool find_next_non_whitespace(GtkDirectionType dir = GTK_DIR_RIGHT);
		virtual Glib::ustring get_word() const;

		virtual bool forward_next_word_start();
		virtual bool backward_next_word_start();
	};
}

#endif

