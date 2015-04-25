//$Id: messagetext.cc 2 2003-01-21 13:41:59Z murrayc $ -*- c++ -*-

/* gtkmm example Copyright (C) 2002 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "messagetext3.h"

using namespace std;

MessageText3::MessageText3()
{
	const string dir_path = Glib::get_user_data_dir();
	 
	Gtk::HBox* cmpLabel = Gtk::manage(new Gtk::HBox());
	cmpLabel->pack_start(*Gtk::manage(new Gtk::Image(Gtk::Stock::COPY, Gtk::ICON_SIZE_MENU)));
	cmpLabel->pack_start(*Gtk::manage(new Gtk::Label("Compare")));
	cmpLabel->set_spacing(2); cmpLabel->show_all();

	Gtk::HBox* chkLabel = Gtk::manage(new Gtk::HBox());
	chkLabel->pack_start(*Gtk::manage(new Gtk::Image(dir_path + "/Hexedit/pixmaps/check-sm.png")));
	chkLabel->pack_start(*Gtk::manage(new Gtk::Label("Checksum")));
	chkLabel->set_spacing(2); chkLabel->show_all();

	Gtk::HBox* fndLabel = Gtk::manage(new Gtk::HBox());
	fndLabel->pack_start(*Gtk::manage(new Gtk::Image(Gtk::Stock::FIND, Gtk::ICON_SIZE_MENU)));
	fndLabel->pack_start(*Gtk::manage(new Gtk::Label("Find")));
	fndLabel->set_spacing(2); fndLabel->show_all();

	Gtk::HBox* bmkLabel = Gtk::manage(new Gtk::HBox());
	bmkLabel->pack_start(*Gtk::manage(new Gtk::Image(dir_path + "/Hexedit/pixmaps/bookmark.png")));
	bmkLabel->pack_start(*Gtk::manage(new Gtk::Label("Bookmarks")));
	bmkLabel->set_spacing(2); bmkLabel->show_all();

	Gtk::HBox* outLabel = Gtk::manage(new Gtk::HBox());
	outLabel->pack_start(*Gtk::manage(new Gtk::Image(Gtk::Stock::PASTE, Gtk::ICON_SIZE_MENU)));
	outLabel->pack_start(*Gtk::manage(new Gtk::Label("Output")));
	outLabel->set_spacing(2); outLabel->show_all();

	m_Label1.set_text("Test");
	append_page(m_Label1, *cmpLabel); 
   append_page(m_Label2, *chkLabel);
   append_page(m_Label3, *fndLabel);
   append_page(m_Label4, *bmkLabel); 
   append_page(m_Label5, *outLabel);	

   set_tab_pos(Gtk::POS_BOTTOM);

}

MessageText3::~MessageText3()
{
}

