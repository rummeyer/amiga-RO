#include "Includes.h"
#include "Startup.h"

/*
**
** InitVars()
**
*/

void InitVars( void )
{
	global_CurrentBank = 0;
	global_DirLoaded[Left_Side] = FALSE;
	global_DirLoaded[Right_Side] = FALSE;
	global_CursorPos[Left_Side] = MUIV_List_Active_Off;
	global_CursorPos[Right_Side] = MUIV_List_Active_Off;

	strcpy( global_IdleStatus, GetCatStr( 40, "Status: idle..." ) );
	strcpy( Status_String, "" );
	strcpy( global_Exec_String, "" );
	strcpy( global_Pattern_String, "#?.info" );
	strcpy( global_ClockFormat, GetCatStr(39, "%s graphics mem, %s other mem, %s total mem" ) );

	strcpy( MenuStrings[0], GetCatStr(6, "Project" ) );
	strcpy( MenuStrings[1], GetCatStr(7, "About..." ) );
	strcpy( MenuStrings[2], GetCatStr(8, "Iconify" ) );
	strcpy( MenuStrings[3], GetCatStr(9, "Quit" ) );
	strcpy( MenuStrings[4], GetCatStr(10, "Options" ) );
	strcpy( MenuStrings[5], GetCatStr(11, "Copy" ) );
	strcpy( MenuStrings[6], GetCatStr(12, "Flags" ) );
	strcpy( MenuStrings[7], GetCatStr(13, "Date" ) );
	strcpy( MenuStrings[8], GetCatStr(14, "Comment" ) );
	strcpy( MenuStrings[9], GetCatStr(15, "Check Diskspace" ) );
	strcpy( MenuStrings[10], GetCatStr(16, "Delete" ) );
	strcpy( MenuStrings[11], GetCatStr(17, "Verify" ) );
	strcpy( MenuStrings[12], GetCatStr(18, "Force" ) );
	strcpy( MenuStrings[13], GetCatStr(19, "Overwrite" ) );
	strcpy( MenuStrings[14], GetCatStr(20, "Always" ) );
	strcpy( MenuStrings[15], GetCatStr(21, "Never" ) );
	strcpy( MenuStrings[16], GetCatStr(22, "Older" ) );
	strcpy( MenuStrings[17], GetCatStr(17, "Verify" ) );
	strcpy( MenuStrings[18], GetCatStr(23, "Settings" ) );
	strcpy( MenuStrings[19], GetCatStr(124, "Check Files" ) );
	strcpy( MenuStrings[20], GetCatStr(25, "Last Message" ) );
	strcpy( MenuStrings[21], GetCatStr(24, "Update All" ) );
}

/*
**
** Init()
**
*/

void Init ( void )
{
	if (!(MUIMasterBase = OpenLibrary(MUIMASTER_NAME,MUIMASTER_VMIN)))
		Fail();

	global_Catalog = OpenCatalog( NULL, "ro.catalog", TAG_DONE );

	InitVars();

	DefaultConfig();
	LoadConfig(TRUE);
	if (!(StartApplication()))
		Fail();
	LoadConfig(FALSE);
	SetMenu();
	Methods();

	DiskObject = GetDiskObject( "PROGDIR:RO" );
	if ( DiskObject ) set(app_RumorOpus, MUIA_Application_DiskObject, DiskObject );

	InstallClock();
}

/*
**
** Fail()
**
*/

void Fail ( void )
{
	RemoveClock();

	if (DiskObject)
		FreeDiskObject( DiskObject );

	StopApplication();

	if ( global_Catalog )
		CloseCatalog( global_Catalog );

	if ( MUIMasterBase )
		CloseLibrary( MUIMasterBase );

	exit(0);
}
