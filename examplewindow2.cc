//$Id: examplewindow.cc 836 2007-05-09 03:02:38Z jjongsma $ -*- c++ -*-

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

#include <iostream>
#include "examplewindow2.h"

ExampleWindow2::ExampleWindow2()
: m_Label1("Contents of tab 1"),
  m_Label2("Contents of tab 2")
{
  set_title("Gtk::Notebook example");
  //set_border_width(3);
  //set_default_size(400, 200);


  add(m_VBox);

  //Add the Notebook, with the button underneath:
  m_Notebook.set_border_width(10);
  m_VBox.pack_start(m_Notebook);
  //m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

  //m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
  //m_Button_Quit.signal_clicked().connect(sigc::mem_fun(*this,
              //&ExampleWindow2::on_button_quit) );

  //Add the Notebook pages:
  m_Notebook.append_page(m_Label1, "Data Inspector");
  m_Notebook.append_page(m_Label2, "Structure Viewer");

  //m_Notebook.signal_switch_page().connect(sigc::mem_fun(*this,
  //            &ExampleWindow2::on_notebook_switch_page) );

  show_all_children();
}

ExampleWindow2::~ExampleWindow2()
{
}

void ExampleWindow2::on_notebook_switch_page(GtkWidget* /* page */, guint page_num)
{
  std::cout << "Switched to tab with index " << page_num << std::endl;

  //You can also use m_Notebook.get_current_page() to get this index.
}
