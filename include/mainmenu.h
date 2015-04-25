#ifndef MAINMENU_H
#define MAINMENU_H

namespace
{
	Glib::ustring ui_info( 
		"<ui>"
		"  <menubar name='MenuBar'>"
		"    <menu action='FileMenu'>"
		"	   <menuitem action='FileNew'/>"
		"	   <menuitem action='FileOpen'/>"
		"      <separator/>"
		"      <menuitem action='FileQuit'/>"
		"    </menu>"
		"    <menu action='EditMenu'>"
		"      <menuitem action='UndoEdit'/>"
		"      <menuitem action='RedoEdit'/>"
		"      <separator/>"
		"      <menuitem action='EditCopy'/>"
		"      <menuitem action='EditPaste'/>"
		"    </menu>"
		"    <menu action='DiskMenu'>"
		"      <menuitem action='DiskOpen'/>"
		"	   <separator/>"
		"      <menuitem action='DiskCopy'/>"
		"    </menu>"
		"    <menu action='OptionsMenu'>"
		"	     <menu action='ByteOrderMenu'>"
		"          <menuitem action='LittleEndian'/>"
		"          <menuitem action='BigEndian'/>"
		"       </menu>"
		"       <menu action='FileOffsetMenu'>"
		"          <menuitem action='FileOffsetHex'/>"
		"          <menuitem action='FileOffsetDec'/>"
		"       </menu>"
		"       <separator/>"
		"	     <menuitem action='ShowToolbar'/>"
		"		  <menuitem action='ShowInspector'/>"
		"		  <menuitem action='ShowResults'/>"
		"       <menuitem action='OptOptions'/>"
		"    </menu>"
		"    <menu action='ToolsMenu'>"
		"       <menu action='ToolsOps'>"
		"          <menuitem action='flip'/>"
		"          <menuitem action='inv'/>"
		"          <menuitem action='shleft'/>"
		"          <menuitem action='shright'/>"
		"          <menuitem action='rotleft'/>"
		"          <menuitem action='rotright'/>"
		"          <menuitem action='blleft'/>"
		"          <menuitem action='blright'/>"
		"          <menuitem action='setceil'/>"
		"          <menuitem action='setfloor'/>"
		"          <separator/>"
		"          <menuitem action='and'/>"
		"          <menuitem action='or'/>"
		"          <menuitem action='xor'/>"
		"          <separator/>"
		"          <menuitem action='change'/>"
		"          <menuitem action='add'/>"
		"          <menuitem action='sub'/>"
		"          <menuitem action='mul'/>"
		"          <menuitem action='div'/>"
		"          <menuitem action='mod'/>"
		"          <separator/>"
		"          <menuitem action='upper'/>"
		"          <menuitem action='lower'/>"
		"          <menuitem action='invcse'/>"
		"       </menu>"
		"       <separator/>"
		"       <menu action='ToolsMrks'>"
		"          <menuitem action='addmrk'/>"
		"          <menuitem action='nextmrk'/>"
		"          <menuitem action='prevmrk'/>"
		"          <separator/>"
		"          <menuitem action='gotomrk'/>"
		"          <menuitem action='seldata'/>"
		"          <menuitem action='editmrk'/>"
		"          <menuitem action='remmrk'/>"
		"          <separator/>"
		"          <menuitem action='opencoll'/>"
		"          <menuitem action='save'/>"
		"          <menuitem action='saveas'/>"
		"          <separator/>"
		"          <menuitem action='clrmrks'/>"
		"          <menuitem action='colprops'/>"
		"       </menu>"
		"       <menuitem action='distrib'/>"
		"       <menu action='comp'>"
		"          <menuitem action='files'/>"
		"          <menuitem action='recomp'/>"
		"          <menuitem action='clrres'/>"
		"          <separator/>"
		"          <menuitem action='next'/>"
		"          <menuitem action='prev'/>"
		"       </menu>"
		"       <menuitem action='Generate'/>"
		"       <menuitem action='FindStr'/>"
		"       <menu action='StructVwer'>"
		"          <menuitem action='selstruct'/>"
		"          <menuitem action='clselib'/>"
		"          <separator/>"
		"          <menuitem action='addstruct'/>"
		"          <menuitem action='remstruct'/>"
		"          <menuitem action='clrstruct'/>"
		"          <separator/>"
		"          <menuitem action='lockstruct'/>"
		"          <menuitem action='fltstruct'/>"
		"          <separator/>"
		"          <menuitem action='gotoaddr'/>"
		"       </menu>"
		"    </menu>"
		"    <menu action='HelpMenu'>"
		"      <menuitem action='AppHelp'/>"
		"      <menuitem action='HelpAbout'/>"
		"    </menu>"
		"  </menubar>"
		"  <toolbar name='ToolBar'>"
 		"    <toolitem action='FileNew'/>"
		"    <toolitem action='FileQuit'/>"
		"    <toolitem action='EditCopy'/>"
		"    <toolitem action='EditPaste'/>"
		"    <toolitem action='OptOptions'/>"
		"  </toolbar>"
		"</ui>");
}

#endif
