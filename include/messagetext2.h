//$Id: messagetext.h 705 2006-07-19 02:55:32Z jjongsma $ -*- c++ -*-

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

#ifndef GTKMM_EXAMPLE_MESSAGETEXT2_H
#define GTKMM_EXAMPLE_MESSAGETEXT2_H

#include <gtkmm.h>

class MessageText2 : public Gtk::Notebook
{
public:
  MessageText2();
  virtual ~MessageText2();

  //virtual void insert_text();

protected:
	Gtk::HBox m_HBoxLabel;
	Gtk::Label m_TabLabel;
	Gtk::Button m_TabBtn;
	Gtk::HBox m_TabBox;

	Gtk::Label m_Label1, m_Label2, m_Label3;
};

#endif //GTKMM_EXAMPLE_MESSAGETEXT_H
