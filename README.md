This is a mimic of Breakpoint Software's Hex Workshop for Windows for the Gnome desktop. After inspecting the few hex editor offerings available for linux I believe this is the best design for such an application. 
I'm doing this not so much as a nessessary application but as an excersise in application design for gnome.

In this app I offer a number of windows designs such as a most recently used file list, drag and drop, and other windows innovations. I'd like to offer this app as a model for other desktop apps for linux as the field right now is thin. I really want to offer Linux as a desktop alternative to Windows, especially if your tired of paying the ever-climbing license fees for windows and osx.

After downloading this code on a gnome desktop the only current assumptions are that you have gtkmm-3.0 and sigc++-2.0 installed on your system. Issuing a "make" with those assumptions should result in a usable application framework.

Features: My own most recent file list, my propeties flat file preferences code.

In lieu of a proper automake tool installation script you'll need to put ./dlgs/Options.glade /usr/share/hexlab, and the copntents of png in /usr/share/hexlab/res. Still to be done is a better, more modular, STL-compliant, design, Gnome lancher.

The make file wiil be replaced with a proper automake conf collection in time, so this code should really be considered total alpha code. This is really important.

===

Update Log:

4/25/2015 - updated to gtkmm 3.0<br>
3/3/2016 - added main hexview widget<br>
12/19/2020 - Much needed refresh, fixed problems with properties class

