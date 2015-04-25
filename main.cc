#include "examplewindow.h"

using namespace std;

int
main(int argc, char** argv)
{
	Gtk::Main kit(argc, argv); 
	ExampleWindow window;	
	Gtk::Main::run(window);
#ifdef _DEBUG
	g_print("We are exiting...\n");
#endif

	return 0;
}

