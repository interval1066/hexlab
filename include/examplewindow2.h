//$Id: examplewindow.h 810 2007-03-30 10:10:25Z murrayc $ -*- c++ -*-

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

#ifndef GTKMM_EXAMPLEWINDOW2_H
#define GTKMM_EXAMPLEWINDOW2_H

#include <gtkmm.h>

class ExampleWindow2 : public Gtk::Window
{
public:
  ExampleWindow2();
  virtual ~ExampleWindow2();

protected:
  //Signal handlers:
  virtual void on_notebook_switch_page(GtkWidget* page, guint page_num);

  //Child widgets:
  Gtk::VBox m_VBox;
  Gtk::Notebook m_Notebook;
  Gtk::Label m_Label1, m_Label2, m_Label3, m_Label4;

  Gtk::HButtonBox m_ButtonBox;
  Gtk::Button m_Button_Quit;
};

#endif //GTKMM_EXAMPLEWINDOW_H
