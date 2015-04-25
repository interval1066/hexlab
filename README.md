# hexlab
This is a mimic of Breakpoint Software's Hex Workshop for Windows for the Gnome desktop. After inspecting the few hex editor offerings available for linux I believe this is the best design for such an application. 
I'm doing this not so much as a nessessary application but as an excersise in application design for gnome.

In this app I offer a number of windows designs such as a most recently used file list, drag and drop, and other windows innovations. I'd like to offer this app as a model for other desktop apps for linux as the field right now is thin. I really want to offer Linux as a desktop alternative to Windows, especially if your tired of paying the ever-climbing license fees for windows and osx.

After downloading this code on a gnome desktop the only current assumptions are that you have gtkmm-2.4 and sigc++-2.0 installed on your system. Issuing a "make" with those assumptions should result in a usable application framework.

The make file wiil be replaced with a proper automake conf collection in time, so this code should really be considered total alpha code. This is really important. If every ready for prime time, it won't be until the code is up-ported to Gnome 3.0, and they way things are going with Gnome, who knows when that will be.

===

Update Log:

4/25/2015 - updated to gtkmm 3.0

