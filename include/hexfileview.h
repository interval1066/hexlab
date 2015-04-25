#ifndef HEXFILEVIEW_H_
#define HEXFILEVIEW_H_

#include <gtkmm.h>

using namespace std;

class HexFileView : public Gtk::Window
{
	gstring m_sFile;
	
public:
	HexFileView(gstring file);
	~HexFileFiew();
	
	gatring GetFileName(void);
	
protected:
	