#include "Includes.h"
#include "Buttons.h"

/*
**
** BankSwitch()
**
*/

void BankSwitch ( void )
{
	if ( global_CurrentBank < global_MaximumBank )
		global_CurrentBank++;
	else
		global_CurrentBank = 0;
	set( ButtonPage, MUIA_Group_ActivePage, global_CurrentBank );
}

/*
**
** HotDir()
**
*/

void HotDir ( int i )
{
	int side;

	if ( strlen ( cfg_HotDir[i] ) > 0 )
	{
		if ( cfg_HotDirs )
			side = Active_Side;
		else
			if ( i < 5 ) side = Left_Side;
			else side = Right_Side;
		
		LoadDirectory( cfg_HotDir[i], side );
	}
}

/*
**
** MenuCommand()
**
*/

void MenuCommand ( int i )
{
	Sleep( TRUE );
	DoFunction( command_entries, Active_Side, cfg_MenuEntry[i] );
	Sleep( FALSE );
}

/*
**
** Button()
**
*/

void Button ( int Num )
{
	ULONG Iconified;
	char * Str = NULL;
	int ErrorNum = ERR_UNSUPPORTED;

	Sleep( TRUE );

	if ( CheckMemory( 50000 ) )
	{
		if ( cfg_ButtonType[Num] == FALSE )
			ErrorNum = DoFunction( command_entries, Active_Side, cfg_ButtonCommand[Num] );
		else
		{
			if(stricmp(cfg_ButtonCommand[Num],"ABOUT")==0)		{ ErrorNum = 0; AboutRequester(); }
			if(stricmp(cfg_ButtonCommand[Num],"DIRECTORY")==0)	{ ErrorNum = 0; set( pg_Page[Active_Side], MUIA_Group_ActivePage, 0 ); }
			if(stricmp(cfg_ButtonCommand[Num],"BUFFERS")==0)	{ ErrorNum = 0; set( pg_Page[Active_Side], MUIA_Group_ActivePage, 1 ); }
			if(stricmp(cfg_ButtonCommand[Num],"VOLUMES")==0)	{ ErrorNum = 0; set( pg_Page[Active_Side], MUIA_Group_ActivePage, 2 ); }
			if(stricmp(cfg_ButtonCommand[Num],"CHANGE")==0)		{ ErrorNum = 0; Change( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"DISK")==0)		{ ErrorNum = 0; DiskInfo( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"SWITCH")==0)		{ ErrorNum = 0; BankSwitch(); }
			if(stricmp(cfg_ButtonCommand[Num],"LCOPY")==0)		{ ErrorNum = 0; ListCopy( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"LSWAP")==0)		{ ErrorNum = 0; ListSwap(); }
			if(stricmp(cfg_ButtonCommand[Num],"LFOLD")==0)		{ ErrorNum = 0; ListFold(); }
			if(stricmp(cfg_ButtonCommand[Num],"LROOT")==0)		{ ErrorNum = 0; LoadRoot( Left_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"RROOT")==0)		{ ErrorNum = 0; LoadRoot( Right_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"ROOT")==0)		{ ErrorNum = 0; LoadRoot( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"LPARENT")==0)	{ ErrorNum = 0; LoadParent( Left_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"RPARENT")==0)	{ ErrorNum = 0; LoadParent( Right_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"PARENT")==0)		{ ErrorNum = 0; LoadParent( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"CURRENT")==0)	{ ErrorNum = 0; LoadDirectory( "", Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"LACTIVE")==0)	{ ErrorNum = 0; ActivateList( Left_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"RACTIVE")==0)	{ ErrorNum = 0; ActivateList( Right_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"LRELOAD")==0)	{ ErrorNum = 0; Reload( Left_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"RRELOAD")==0)	{ ErrorNum = 0; Reload( Right_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"RELOAD")==0)		{ ErrorNum = 0; Reload( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"REBOOT")==0)		{ ErrorNum = 0; ColdReboot(); }
			if(stricmp(cfg_ButtonCommand[Num],"UPDATE")==0)		{ ErrorNum = 0; Update( Active_Side ); UpdateNumFiles( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"BYNAME")==0)		{ ErrorNum = 0; Select( Active_Side ); UpdateNumFiles( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"ICON")==0)		{ ErrorNum = 0; Icon( Active_Side ); UpdateNumFiles( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"SHRINK")==0)		{ ErrorNum = 0; Shrink( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"EXPAND")==0)		{ ErrorNum = 0; Expand( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"ALL")==0)		{ ErrorNum = 0; DoMethod( lv_Directory[Active_Side], MUIM_List_Select, MUIV_List_Select_All, MUIV_List_Select_On, NULL ); UpdateNumFiles( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"NONE")==0)		{ ErrorNum = 0; set( lv_Directory[Active_Side], MUIA_List_Active, MUIV_List_Active_Off ); DoMethod( lv_Directory[Active_Side], MUIM_List_Select, MUIV_List_Select_All, MUIV_List_Select_Off, NULL ); UpdateNumFiles( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"TOGGLE")==0)		{ ErrorNum = 0; DoMethod( lv_Directory[Active_Side], MUIM_List_Select, MUIV_List_Select_All, MUIV_List_Select_Toggle, NULL ); UpdateNumFiles( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"HELP")==0)		{ ErrorNum = 0; DoMethod( app_RumorOpus, MUIM_Application_ShowHelp, NULL, "RO.guide", "main", 0 ); }
			if(stricmp(cfg_ButtonCommand[Num],"QUIT")==0)		{ ErrorNum = 0; DoMethod( app_RumorOpus, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit ); }
			if(stricmp(cfg_ButtonCommand[Num],"BYTES")==0)		{ ErrorNum = 0; Str = Bytes( Active_Side ); }
			if(stricmp(cfg_ButtonCommand[Num],"FIT")==0)		{ ErrorNum = 0; Str = Fit( Active_Side, TRUE ); }

			if(stricmp(cfg_ButtonCommand[Num],"ASSIGN")==0)		ErrorNum = MakeAssign( Active_Side );
			if(stricmp(cfg_ButtonCommand[Num],"MAKEDIR")==0)	ErrorNum = CreateDirectory( Active_Side, FALSE );
			if(stricmp(cfg_ButtonCommand[Num],"MDLOAD")==0)		ErrorNum = CreateDirectory( Active_Side, TRUE );
			if(stricmp(cfg_ButtonCommand[Num],"RELABEL")==0)	ErrorNum = RelabelDevice( Active_Side );
			if(stricmp(cfg_ButtonCommand[Num],"DELETE")==0)		ErrorNum = DoFunction( delete_entries,  Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"COPY")==0)		ErrorNum = DoFunction( copy_entries,    Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"COPYAS")==0)		ErrorNum = DoFunction( copyas_entries,  Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"MOVE")==0)		ErrorNum = DoFunction( move_entries,    Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"MOVEAS")==0)		ErrorNum = DoFunction( moveas_entries,  Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"ACTION")==0)		ErrorNum = DoFunction( action_entries,  Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"COPYDEV")==0)	ErrorNum = DoFunction( copydev_entries, Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"TOUCH")==0)		ErrorNum = DoFunction( touch_entries,   Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"RENAME")==0)		ErrorNum = DoFunction( rename_entries,  Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"DUP")==0)		ErrorNum = DoFunction( dup_entries,     Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"SETDATE")==0)	ErrorNum = DoFunction( setdate_entries, Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"NOTE")==0)		ErrorNum = DoFunction( comment_entries, Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"INFO")==0)		ErrorNum = DoFunction( info_entries,    Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"LISTARC")==0)	ErrorNum = DoFunction( listarc_entries, Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"UNARC")==0)		ErrorNum = DoFunction( unarc_entries,   Active_Side, NULL );
			if(stricmp(cfg_ButtonCommand[Num],"PROTECT")==0)	ErrorNum = DoFunction( protect_entries, Active_Side, NULL );
		}
	}
	else
		ErrorNum = ERR_NO_MEMORY;

	if ( ErrorNum != 0 )
		Str = Error( ErrorNum );

	if ( Str != NULL )
	{
		if ( !global_ConfigChange )
		{
			SleepClock( TRUE );
			SleepClock( TRUE );
		}
		strcpy( Status_String, Str );
		DoMethod( app_RumorOpus, MUIM_Application_ReturnID, ID_Message );
		get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
		if ( Iconified )
			DisplayBeep( 0 );
	}

	Sleep( FALSE );
}
