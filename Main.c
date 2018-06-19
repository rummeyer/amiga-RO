#include "Includes.h"
#include "Main.h"

/*
**
** RO -- MUI-Based FileManager, Shareware
**
** Copyright © 1994-2018 by Oliver Rummeyer
**
*/

int main ( int argc, char *argv[] )
{
	int Once = 1;
	ULONG signal, id, Opened, Page;
	struct Window * IntuiWindow;
	int ErrorNum, i;
	BOOL BOOL_ReloadConfig = FALSE;
	BPTR lock;
	__aligned struct FileInfoBlock * fib;
	char * Buffer, * String;
	char PrefsName[512];

	global_ARGC = argc;
	global_ARGV = argv;

	CheckKey();

	if (global_Special)
		global_KeyFile = TRUE;

	Init();

	strcpy( global_Path[Left_Side], "" );
	strcpy( global_Path[Right_Side], "" );

	if ( argc == 0 )
	{
		argc = _WBArgc;
		argv = _WBArgv;
	}

	lock = Lock( "PROGDIR:", ACCESS_READ );
	if ( lock )
	{
		strcpy( PrefsName, "" );
		NameFromLock( lock, PrefsName, sizeof( PrefsName ) );
		AddPart( PrefsName, "ROPrefs", sizeof( PrefsName ) );
		UnLock( lock );
	}

	for( i = argc; i > 0; i--)
		if ( argv[i] && stricmp( argv[i], "ICONIFY" ) == 0 )
			set( app_RumorOpus, MUIA_Application_Iconified, 1 );

	get( app_RumorOpus, MUIA_Application_Base, &String );
	strcpy( global_ARexxName, String );

	set( wi_Main, MUIA_Window_Open, TRUE );

	while ( !global_QuitProgram )
	{
		id = DoMethod( app_RumorOpus, MUIM_Application_Input, &signal );
		if ( Once == 2)
		{
			if ( strlen( cfg_LeftLoad ) > 0 ) LoadDirectory( cfg_LeftLoad, Left_Side );
			if ( strlen( cfg_RightLoad ) > 0 ) LoadDirectory( cfg_RightLoad, Right_Side );
			global_SleepClock = 0;
			UpdateClock();
			set( wi_Main, MUIA_Window_ActiveObject, lv_Directory[Left_Side]);
			Once=3;
		}
		if ( ( id > ID_Start ) && ( id < ID_Stop ) && ( id != ID_UpdateTextLeft ) && ( id != ID_UpdateTextRight ) && ( id != ID_Sleep ) && ( id != ID_WakeUp ) && (id != ID_Message) ) SleepClock( FALSE );
		switch( id )
		{

			case MUIV_Application_ReturnID_Quit:
				global_QuitProgram = QuitRequester();
				break;

			/*** Menu Handling ***/

			case ID_About:		AboutRequester();										break;
			case ID_AboutMUI:	DoMethod( app_RumorOpus, MUIM_Application_AboutMUI, wi_Main ); break;
			case ID_Iconify:	set( app_RumorOpus, MUIA_Application_Iconified, 1 );	break;
			case ID_CopyFlags:	cfg_CopyFlags = !cfg_CopyFlags;							break;
			case ID_CopyDate:	cfg_CopyDate = !cfg_CopyDate;							break;
			case ID_CopyNote:	cfg_CopyNote = !cfg_CopyNote;							break;
			case ID_CheckSpace:	cfg_CheckFit = !cfg_CheckFit;							break;
			case ID_CopyVerify:	cfg_CopyVerify = !cfg_CopyVerify;						break;
			case ID_Verify:		cfg_DelVerify = !cfg_DelVerify;							break;
			case ID_Force:		if ( cfg_DelForce == 1 ) cfg_DelForce = 0;	else cfg_DelForce = 1;	break;
			case ID_OverYes:	cfg_Overwrite = 0;										break;
			case ID_OverNo:		cfg_Overwrite = 1;										break;
			case ID_OverVerify:	cfg_Overwrite = 2;										break;
			case ID_OverOlder:	cfg_Overwrite = 3;										break;
			case ID_UpdateAll:	Reload(Left_Side);	Reload(Right_Side);					break;

			case ID_Message:
				if ( strlen( Status_String ) > 0 )
				{
					SleepClock ( TRUE );
					set( bt_StatusBar, MUIA_Text_Contents, Status_String );
				}
				break;

			case ID_MUIPrefs:
				DoMethod( app_RumorOpus, MUIM_Application_OpenConfigWindow, 0 );
				break;

			case ID_ROPrefs:
				SleepWindow( TRUE );
				if( Exists( PrefsName ) )
					i = ExecuteCommandNoOutput( PrefsName, " " );
				else
					if( Exists( "SYS:Prefs/ROPrefs" ) )
						i = ExecuteCommandNoOutput( "SYS:Prefs/ROPrefs", " " );
					else
						if( Exists( "SYS:Prefs/RO" ) )
							i = ExecuteCommandNoOutput( "SYS:Prefs/RO", " " );
						else
						{
							i = 10;
							DisplayBeep(0);
						}
				if ( i == 0 )
					BOOL_ReloadConfig = TRUE;
				SleepWindow( FALSE );
				break;


			/*** Directory Handling ***/

			case ID_DoubleClick:
				DoMethod( lv_Directory[Active_Side], MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &fib );
				get( lv_Directory[Active_Side], MUIA_Dirlist_Path, &Buffer );
				String = strdup( Buffer );
				if ( String )
				{
					if ( fib -> fib_DirEntryType > 0 )
						LoadDirectory( String, Active_Side );
					else
					{
						SleepClock( TRUE );
						sprintf( Status_String, GetCatStr( 47, "Performing FileAction for '%s'..." ), String );
						set( bt_StatusBar, MUIA_Text_Contents, Status_String );
						ErrorNum = FileAction( String );
						if ( ErrorNum != 0 )
							DisplayBeep( 0 );
						global_AskOnce = FALSE;
						SleepClock( FALSE );
					}
					free( String );
				}
				break;

			case ID_StringEnterLeft:
				Buffer = GetPath( Left_Side );
				if ( LoadDirectory( Buffer, Left_Side ) != 0 )
				{
					String = FindMatch( Buffer );
					if ( String )
						LoadDirectory( String, Left_Side );
					else
						set( st_PathGadget[Left_Side], MUIA_String_Contents, global_Path[Left_Side] );
				}
				set( wi_Main, MUIA_Window_ActiveObject, st_PathGadget[Left_Side] );
				break;

			case ID_StringEnterRight:
				Buffer = GetPath( Right_Side );
				if ( LoadDirectory( Buffer, Right_Side ) != 0 )
				{
					String = FindMatch( Buffer );
					if ( String )
						LoadDirectory( String, Right_Side );
					else
						set( st_PathGadget[Right_Side], MUIA_String_Contents, global_Path[Right_Side] );
				}
				set( wi_Main, MUIA_Window_ActiveObject, st_PathGadget[Right_Side] );
				break;
					
			/*** ARexx Events ***/

			case ID_Sleep:				Sleep(TRUE);				break;
			case ID_WakeUp:				Sleep(FALSE);				break;

			/*** General Events ***/

			case ID_BufferLeft:			LoadBuffer(Left_Side);					break;
			case ID_BufferRight:		LoadBuffer(Right_Side);					break;
			case ID_VolumeLeft:			LoadVolume(Left_Side);					break;
			case ID_VolumeRight:		LoadVolume(Right_Side);					break;
			case ID_StatusLeft:			LoadParent(Left_Side);					break;
			case ID_StatusRight:		LoadParent(Right_Side);					break;
			case ID_UpdateTextLeft:		UpdateStatusText(Left_Side);			break;
			case ID_UpdateTextRight:	UpdateStatusText(Right_Side);			break;
			case ID_LeftActive:     	SetActiveColor(Left_Side);				break;
			case ID_RightActive:     	SetActiveColor(Right_Side);				break;
			case ID_PageChange:     	SetActiveColor(Active_Side);			break;
			case ID_LeftChange:
				if ( Active_Side != Left_Side )
					ActivateList(Left_Side);
				UpdateNumFiles(Left_Side);
				break;
			case ID_RightChange:
				if ( Active_Side != Right_Side )
					ActivateList(Right_Side);
				UpdateNumFiles(Right_Side);
				break;
			case ID_TildeCycle:			ActivateList(OtherSide(Active_Side));	break;
			case ID_StatusBar:			BankSwitch();							break;

			/*** HotDir Keys ***/

			case ID_Hot_0:				HotDir(0); 							break;
			case ID_Hot_1:				HotDir(1); 							break;
			case ID_Hot_2:				HotDir(2); 							break;
			case ID_Hot_3:				HotDir(3); 							break;
			case ID_Hot_4:				HotDir(4); 							break;
			case ID_Hot_5:				HotDir(5); 							break;
			case ID_Hot_6:				HotDir(6); 							break;
			case ID_Hot_7:				HotDir(7); 							break;
			case ID_Hot_8:				HotDir(8); 							break;
			case ID_Hot_9:				HotDir(9);	 						break;

			/*** Other HotKeys ***/

			case ID_Key_0:				Button(120);						break;
			case ID_Key_1:				Button(121); 						break;
			case ID_Key_2:				Button(122); 						break;
			case ID_Key_3:				Button(123); 						break;
			case ID_Key_4:				Button(124); 						break;
			case ID_Key_5:				Button(125); 						break;
			case ID_Key_6:				Button(126); 						break;
			case ID_Key_7:				Button(127); 						break;
			case ID_Key_8:				Button(128); 						break;
			case ID_Key_9:				Button(129);						break;
			case ID_Key_10:				Button(130);						break;
			case ID_Key_11:				Button(131); 						break;
			case ID_Key_12:				Button(132); 						break;
			case ID_Key_13:				Button(133); 						break;
			case ID_Key_14:				Button(134); 						break;
			case ID_Key_15:				Button(135); 						break;
			case ID_Key_16:				Button(136); 						break;
			case ID_Key_17:				Button(137); 						break;
			case ID_Key_18:				Button(138); 						break;
			case ID_Key_19:				Button(139);						break;
			case ID_Key_20:				Button(140);						break;
			case ID_Key_21:				Button(141); 						break;
			case ID_Key_22:				Button(142); 						break;
			case ID_Key_23:				Button(143); 						break;
			case ID_Key_24:				Button(144); 						break;
			case ID_Key_25:				Button(145); 						break;
			case ID_Key_26:				Button(146); 						break;
			case ID_Key_27:				Button(147); 						break;
			case ID_Key_28:				Button(148); 						break;
			case ID_Key_29:				Button(149);						break;
			case ID_Key_30:				Button(150);						break;
			case ID_Key_31:				Button(151); 						break;
			case ID_Key_32:				Button(152); 						break;
			case ID_Key_33:				Button(153); 						break;
			case ID_Key_34:				Button(154); 						break;
			case ID_Key_35:				Button(155); 						break;
			case ID_Key_36:				Button(156); 						break;
			case ID_Key_37:				Button(157); 						break;
			case ID_Key_38:				Button(158); 						break;
			case ID_Key_39:				Button(159);						break;
			case ID_Key_40:				Button(160);						break;
			case ID_Key_41:				Button(161); 						break;
			case ID_Key_42:				Button(162); 						break;
			case ID_Key_43:				Button(163); 						break;
			case ID_Key_44:				Button(164); 						break;
			case ID_Key_45:				Button(165); 						break;
			case ID_Key_46:				Button(166); 						break;
			case ID_Key_47:				Button(167); 						break;
			case ID_Key_48:				Button(168); 						break;
			case ID_Key_49:				Button(169);						break;

			/*** UserMenu Commands ***/

			case ID_Menu_0:				MenuCommand(0);						break;
			case ID_Menu_1:				MenuCommand(1);						break;
			case ID_Menu_2:				MenuCommand(2);						break;
			case ID_Menu_3:				MenuCommand(3);						break;
			case ID_Menu_4:				MenuCommand(4);						break;
			case ID_Menu_5:				MenuCommand(5);						break;
			case ID_Menu_6:				MenuCommand(6);						break;
			case ID_Menu_7:				MenuCommand(7);						break;
			case ID_Menu_8:				MenuCommand(8);						break;
			case ID_Menu_9:				MenuCommand(9);						break;
			case ID_Menu_10:			MenuCommand(10);					break;
			case ID_Menu_11:			MenuCommand(11);					break;
			case ID_Menu_12:			MenuCommand(12);					break;
			case ID_Menu_13:			MenuCommand(13);					break;
			case ID_Menu_14:			MenuCommand(14);					break;
			case ID_Menu_15:			MenuCommand(15);					break;
			case ID_Menu_16:			MenuCommand(16);					break;
			case ID_Menu_17:			MenuCommand(17);					break;
			case ID_Menu_18:			MenuCommand(18);					break;
			case ID_Menu_19:			MenuCommand(19);					break;
		}

		get( wi_Main, MUIA_Window_Open, &Opened );

		if ( Opened )
			{
			get( wi_Main, MUIA_Window_Window, &IntuiWindow );
			if ( IntuiWindow )
				if ( IntuiWindow->MessageKey )

					if ( IntuiWindow->MessageKey->Class == IDCMP_MOUSEBUTTONS )
					{
						if ( IntuiWindow->MessageKey->Code == SELECTDOWN )
							if ( ( i = MouseSide( IntuiWindow->MouseX, IntuiWindow->MouseY ) ) != -1 )
								if ( i != Active_Side && i != -2 )
								{
									get( pg_Page[i], MUIA_Group_ActivePage, &Page );
									if( Page == 0 ) ActivateList( i );
								}

						if ( IntuiWindow->MessageKey->Code == MIDDLEDOWN )
							if ( ( i = MouseSide( IntuiWindow->MouseX, IntuiWindow->MouseY ) ) != -1 )
							{
								if ( i != -2 )
								{
									if ( cfg_MiddleMouse == 1 || cfg_MiddleMouse == 3 )
									{
										get( pg_Page[i], MUIA_Group_ActivePage, &Page );
										if( Page == 2 )
											set( pg_Page[i], MUIA_Group_ActivePage, 0 );
										else
											set( pg_Page[i], MUIA_Group_ActivePage, MUIV_Group_ActivePage_Next );
									}
								}
								else
									if ( cfg_MiddleMouse == 2 || cfg_MiddleMouse == 3 ) BankSwitch();
							}
					}
			}

		if ( signal && !global_QuitProgram ) Wait(signal);
		if ( Once == 1 )
		{
			if ( !global_KeyFile )
				AboutRequester();
			Once = 2;
		}
		if ( Once == 3 )
		{
			set( pg_Page[Left_Side], MUIA_Group_ActivePage, cfg_LeftPage );
			set( pg_Page[Right_Side], MUIA_Group_ActivePage, cfg_RightPage );
			SleepClock(TRUE);
			SleepClock(FALSE);
			Once = 0;
		}

		if ( BOOL_ReloadConfig )
		{
			set( wi_Main, MUIA_Window_Open, FALSE );
			SleepClock(TRUE);

			if (DiskObject)
				FreeDiskObject( DiskObject );

			StopApplication();

			DefaultConfig();
			LoadConfig(TRUE);
			if (!(StartApplication()))
				Fail();
			LoadConfig(FALSE);
			SetMenu();
			Methods();

			DiskObject = GetDiskObject( "PROGDIR:RO" );
			if ( DiskObject ) set(app_RumorOpus, MUIA_Application_DiskObject, DiskObject );

			if ( global_DirLoaded[Left_Side] )
				LoadDirectory( global_Path[Left_Side], Left_Side );
			if ( global_DirLoaded[Right_Side] )
				LoadDirectory( global_Path[Right_Side], Right_Side );

			SleepClock(FALSE);
			set( wi_Main, MUIA_Window_Open, TRUE );

			BOOL_ReloadConfig = FALSE;
			global_ConfigChange = TRUE;
		}
	}

	if ( !global_KeyFile )
		AboutRequester();

	set( wi_Main, MUIA_Window_Open, FALSE );

	Fail();
}
