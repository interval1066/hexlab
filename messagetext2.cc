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

#include "messagetext2.h"

using namespace std;

MessageText2::MessageText2()
{
	const string dir_path = Glib::get_user_data_dir();
	Gtk::HBox* cmpLabel = Gtk::manage(new Gtk::HBox());
	cmpLabel->pack_start(*Gtk::manage(new Gtk::Image(dir_path + "/Hexedit/pixmaps/glass.png")));
	 
	cmpLabel->pack_start(*Gtk::manage(new Gtk::Label("Data Inspector")));
	cmpLabel->set_spacing(2);
	cmpLabel->show_all();
	 
	m_Label1.set_text("Test");
	append_page(m_Label1, *cmpLabel);
	Gtk::HBox* stLabel = Gtk::manage(new Gtk::HBox());

	stLabel->pack_start(*Gtk::manage(new Gtk::Image(dir_path + "/Hexedit/pixmaps/struct.png")));
	stLabel->pack_start(*Gtk::manage(new Gtk::Label("Structure Viewer")));
	 
	cmpLabel->set_spacing(2);
	cmpLabel->show_all();
	stLabel->set_spacing(2);
	 
	stLabel->show_all();
	m_Label1.set_text("Test");
	m_Label2.set_text("Test");

	append_page(m_Label2, *stLabel);
	set_tab_pos(Gtk::POS_BOTTOM);
}

MessageText2::~MessageText2()
{
}

