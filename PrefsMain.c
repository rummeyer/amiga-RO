#include "PrefsIncludes.h"
#include "PrefsMain.h"

/*
**
** ROPrefs -- MUI-Based FileManager Prefs, Shareware
**
** Copyright © 1994-1998 by Oliver Rummeyer
**
*/

int main ( int argc, char *argv[] )
{
	ULONG signal, id, active, Bactive, dummy, i;
	BPTR dirlock;
	char filename[256];
	struct FileRequester * req;
	char * str;

	CheckKey();

	if (global_Special)
		global_KeyFile = TRUE;

	Init();

	set( wi_Main, MUIA_Window_Open, TRUE );

	if ( !global_KeyFile )
	{
		AboutRequester();
		set( bt_Save, MUIA_Disabled, TRUE );
		DoMethod( wi_Main, MUIM_Window_SetMenuState, ID_MenuSaveAs, FALSE );
	}

	DefaultConfig();
	LoadConfig("ENV:RO/RO.prefs");

	while ( !global_QuitProgram )
	{
		id = DoMethod( app_RumorOpusPrefs, MUIM_Application_Input, &signal );
		switch( id )
		{

			/*** Main buttons ***/

			case MUIV_Application_ReturnID_Quit:
				global_QuitProgram = TRUE;
				global_Cancel = TRUE;
				break;

			case ID_MUIPrefs:
				DoMethod( app_RumorOpusPrefs, MUIM_Application_OpenConfigWindow, 0 );
				break;

			case ID_Save_Btn:
				dirlock = CreateDir( "ENV:RO" );
				if ( dirlock )
					UnLock( dirlock );
				if(!SaveConfig("ENV:RO/RO.prefs"))
					DisplayBeep(0);
				dirlock = CreateDir( "ENVARC:RO" );
				if ( dirlock )
					UnLock( dirlock );
				if(!SaveConfig("ENVARC:RO/RO.prefs"))
					DisplayBeep(0);
				global_QuitProgram = TRUE;
				break;

			case ID_Use_Btn:
				dirlock = CreateDir( "ENV:RO" );
				if ( dirlock )
					UnLock( dirlock );
				if(!SaveConfig("ENV:RO/RO.prefs"))
					DisplayBeep(0);
				global_QuitProgram = TRUE;
				break;

			/*** Menu functions ***/

			case ID_MenuOpen:
				req=MUI_AllocAslRequestTags(ASL_FileRequest,TAG_DONE);
				if(req)
				{
					if(MUI_AslRequestTags(req, ASLFO_TitleText, "Load Preferences", ASLFR_InitialDrawer, "SYS:Prefs/Presets", TAG_DONE))
					{
						strncpy(filename, req->fr_Drawer, sizeof(filename)-1);
						AddPart(filename, req->fr_File, sizeof(filename)-1);
						DefaultConfig();
						LoadConfig(filename);
					}
				    MUI_FreeAslRequest(req);
				}
				break;

			case ID_MenuSaveAs:
				req=MUI_AllocAslRequestTags(ASL_FileRequest,TAG_DONE);
				if(req)
				{
					if(MUI_AslRequestTags(req, ASLFO_TitleText, "Save Preferences", ASLFR_InitialDrawer, "SYS:Prefs/Presets",TAG_DONE))
					{
						strncpy(filename, req->fr_Drawer, sizeof(filename)-1);
						AddPart(filename, req->fr_File, sizeof(filename)-1);
						if(!SaveConfig(filename))
							DisplayBeep(0);
					}
				    MUI_FreeAslRequest(req);
				}
				break;

			case ID_MenuIconify:
				set( app_RumorOpusPrefs, MUIA_Application_Iconified, 1 );
				break;

			case ID_MenuAbout:
				AboutRequester();
				break;

			case ID_MenuReset:
				DefaultConfig();
				break;

			case ID_MenuLast:
				DefaultConfig();
				LoadConfig("ENVARC:RO/RO.prefs");
				break;

			case ID_MenuRestore:
				DefaultConfig();
				LoadConfig("ENV:RO/RO.prefs");
				break;

			case ID_FormatLeftEnter:
				get( st_FormatLeft, MUIA_String_Contents, &str );
				if( strlen( str ) < 6 )
					set( bt_FormatLeft, MUIA_Disabled, FALSE );
				else
					set( bt_FormatLeft, MUIA_Disabled, TRUE );

			case ID_FormatRightEnter:
				get( st_FormatRight, MUIA_String_Contents, &str );
				if( strlen( str ) < 6 )
					set( bt_FormatRight, MUIA_Disabled, FALSE );
				else
					set( bt_FormatRight, MUIA_Disabled, TRUE );

			/*** Input events page ***/

			case ID_EventSelect:
				get( lv_Input, MUIA_List_Active, &active );
				DoMethod( lv_Input, MUIM_List_GetEntry, active, &str );
				set( st_Input, MUIA_String_Contents, str );

				set( cy_Event, MUIA_Cycle_Active, cfg_Event[active] );
				if( ( cfg_Event[active]==0 ) || ( cfg_Event[active]==1 ) || ( cfg_Event[active]==3 ) )
				{
					if ( cfg_Type[active] )
					{
						set( cy_Type, MUIA_Cycle_Active, 1 );
						set( st_Command, MUIA_String_Contents, cfg_String[active] );
					}
					else
					{
						set( cy_Type, MUIA_Cycle_Active, 0 );
						set( st_Function, MUIA_String_Contents, cfg_String[active] );
					}
				}
				if( cfg_Event[active]==2 )
					set( st_Directory, MUIA_String_Contents, cfg_String[active] );
				break;

			case ID_DirEnter:
				get( st_Directory, MUIA_String_Contents, &str );
				strcpy( cfg_String[active], str );
				break;

			case ID_FunctionEnter:
				get( st_Function, MUIA_String_Contents, &str );
				strcpy( cfg_String[active], str );
				break;

			case ID_CommandEnter:
				get( st_Command, MUIA_String_Contents, &str );
				strcpy( cfg_String[active], str );
				break;

			case ID_EventtypeChange:
				get( cy_Event, MUIA_Cycle_Active, &dummy );
				cfg_Event[active]=dummy;
				if ( dummy == 0 || dummy == 1 || dummy == 3 )
				{
					if( dummy == 3 )
					{
						set( cy_Type, MUIA_Disabled, TRUE );
						set( cy_Type, MUIA_Cycle_Active, 1 );
					}
					else
						set( cy_Type, MUIA_Disabled, FALSE );
					get( cy_Type, MUIA_Cycle_Active, &i );
					if ( i == 1 )
					{
						set( pp_Function, MUIA_Disabled, TRUE );
						set( pp_Command, MUIA_Disabled, FALSE );
						get( st_Command, MUIA_String_Contents, &str );
					}
					else
					{
						set( pp_Function, MUIA_Disabled, FALSE );
						set( pp_Command, MUIA_Disabled, TRUE );
						get( st_Function, MUIA_String_Contents, &str );
					}
					set( pp_Directory, MUIA_Disabled, TRUE );
				}
				else
				{
					get( st_Directory, MUIA_String_Contents, &str );
					set( cy_Type, MUIA_Disabled, TRUE );
					set( pp_Function, MUIA_Disabled, TRUE );
					set( pp_Command, MUIA_Disabled, TRUE );
					if ( dummy == 2 )
						set( pp_Directory, MUIA_Disabled, FALSE );
					else
						set( pp_Directory, MUIA_Disabled, TRUE );
				}
				strcpy( cfg_String[active], str );
				break;

			case ID_CommandtypeChange:
				get( cy_Type, MUIA_Cycle_Active, &dummy );
				cfg_Type[active]=dummy;
				if ( dummy == 1 )
				{
					set( pp_Function, MUIA_Disabled, TRUE );
					set( pp_Command, MUIA_Disabled, FALSE );
					get( st_Command, MUIA_String_Contents, &str );
				}
				else
				{
					set( pp_Function, MUIA_Disabled, FALSE );
					set( pp_Command, MUIA_Disabled, TRUE );
					get( st_Function, MUIA_String_Contents, &str );
				}
				strcpy( cfg_String[active], str );
				break;

			case ID_EnterEvent:
				get( st_Input, MUIA_String_Contents, &str );
				set( lv_Input, MUIA_List_Quiet, TRUE );
				DoMethod( lv_Input, MUIM_List_InsertSingle, str, MUIV_List_Insert_Bottom );
				DoMethod( lv_Input, MUIM_List_Exchange, active, MUIV_List_Exchange_Bottom );
				DoMethod( lv_Input, MUIM_List_Remove, MUIV_List_Remove_Last );
				set( lv_Input, MUIA_List_Quiet, FALSE );
				break;

			case ID_AddEvent:
				DoMethod( lv_Input, MUIM_List_InsertSingle, "", MUIV_List_Insert_Bottom );
				get( lv_Input, MUIA_List_Entries, &dummy );
				strcpy( cfg_String[dummy], "" );
				set( st_Directory, MUIA_String_Contents, "" );
				cfg_Event[dummy]=0;
				cfg_Type[dummy]=0;
				set( pp_Command, MUIA_Disabled, TRUE );
				set( pp_Directory, MUIA_Disabled, TRUE );
				set( cy_Event, MUIA_Disabled, FALSE );
				set( pp_Function, MUIA_Disabled, FALSE );
				set( cy_Type, MUIA_Disabled, FALSE );
				set( lv_Input, MUIA_List_Active, dummy );
				if( dummy < 220 )
					set( bt_AddEvent, MUIA_Disabled, FALSE );
				else
					set( bt_AddEvent, MUIA_Disabled, TRUE );
				set( wi_Main, MUIA_Window_ActiveObject, st_Input );
				break;

			case ID_DelEvent:
				get( lv_Input, MUIA_List_Entries, &dummy );
				DoMethod( lv_Input, MUIM_List_Remove, MUIV_List_Remove_Active );
				for( i=active; i<dummy; i++ )
				{
					strcpy( cfg_String[i], cfg_String[i+1] );
					cfg_Event[i]=cfg_Event[i+1];
					cfg_Type[i]=cfg_Type[i+1];
				}
				break;

			/*** Filetypes page ***/

			case ID_FiletypeSelect:
				get( lv_Filetypes, MUIA_List_Active, &Bactive );
				DoMethod( lv_Filetypes, MUIM_List_GetEntry, Bactive, &str );
				set( st_Filetypes, MUIA_String_Contents, str );
				if( Bactive < 3 )
				{
					set( cy_Filetype, MUIA_Disabled, TRUE );
					set( pp_CommandB, MUIA_Disabled, TRUE );
					set( st_CommandA, MUIA_String_Contents, cfg_FileType[Bactive] );
				}
				else
				{
					set( cy_Filetype, MUIA_Disabled, FALSE );
					set( bt_DelFiletype, MUIA_Disabled, FALSE );
					set( st_CommandA, MUIA_String_Contents, cfg_RecogCommand[Bactive-3] );
					set( st_Recog, MUIA_String_Contents, cfg_RecogHex[Bactive-3] );
					set( st_Pattern, MUIA_String_Contents, cfg_RecogString[Bactive-3] );

					if( cfg_RecogType[Bactive-3] )
					{
						set( pp_CommandB, MUIA_Disabled, FALSE );
						set( cy_Filetype, MUIA_Cycle_Active, 1 );
						set( st_CommandB, MUIA_String_Contents, cfg_RecogCommandB[Bactive-3] );
					}
					else
					{
						set( pp_CommandB, MUIA_Disabled, TRUE );
						set( cy_Filetype, MUIA_Cycle_Active, 0 );
					}
				}
				break;

			case ID_FiletypeDisabled:
				set( st_CommandB, MUIA_String_Contents, "" );
				set( st_Recog, MUIA_String_Contents, "" );
				set( st_Pattern, MUIA_String_Contents, "" );
				set( bt_DelFiletype, MUIA_Disabled, TRUE );
				set( st_Recog, MUIA_Disabled, TRUE );
				set( st_Pattern, MUIA_Disabled, TRUE );
				break;

			case ID_FiletypeEnabled:
				set( bt_DelFiletype, MUIA_Disabled, FALSE );
				set( cy_Filetype, MUIA_Disabled, FALSE );
				set( st_Recog, MUIA_Disabled, FALSE );
				set( st_Pattern, MUIA_Disabled, FALSE );
				break;

			case ID_AddFiletype:
				DoMethod( lv_Filetypes, MUIM_List_InsertSingle, "", MUIV_List_Insert_Bottom );
				get( lv_Filetypes, MUIA_List_Entries, &dummy );
				strcpy( cfg_RecogHex[dummy-4], "" );
				strcpy( cfg_RecogString[dummy-4], "" );
				strcpy( cfg_RecogCommand[dummy-4], "" );
				strcpy( cfg_RecogCommandB[dummy-4], "" );
				set( st_CommandB, MUIA_String_Contents, "" );
				cfg_RecogType[dummy-4]=FALSE;
				set( lv_Filetypes, MUIA_List_Active, dummy );
				if( dummy < 100 )
					set( bt_AddFiletype, MUIA_Disabled, FALSE );
				else
					set( bt_AddFiletype, MUIA_Disabled, TRUE );
				set( wi_Main, MUIA_Window_ActiveObject, st_Filetypes );
				break;

			case ID_DelFiletype:
				get( lv_Filetypes, MUIA_List_Entries, &dummy );
				DoMethod( lv_Filetypes, MUIM_List_Remove, MUIV_List_Remove_Active );
				for( i=Bactive-3; i<dummy; i++ )
				{
					strcpy( cfg_RecogHex[i], cfg_RecogHex[i+1] );
					strcpy( cfg_RecogString[i], cfg_RecogString[i+1] );
					strcpy( cfg_RecogCommand[i], cfg_RecogCommand[i+1] );
					strcpy( cfg_RecogCommandB[i], cfg_RecogCommandB[i+1] );
					cfg_RecogType[i]=cfg_RecogType[i+1];
				}
				break;

			case ID_FiletypeEnter:
				get( st_Filetypes, MUIA_String_Contents, &str );
				set( lv_Filetypes, MUIA_List_Quiet, TRUE );
				DoMethod( lv_Filetypes, MUIM_List_InsertSingle, str, MUIV_List_Insert_Bottom );
				DoMethod( lv_Filetypes, MUIM_List_Exchange, Bactive, MUIV_List_Exchange_Bottom );
				DoMethod( lv_Filetypes, MUIM_List_Remove, MUIV_List_Remove_Last );
				set( lv_Filetypes, MUIA_List_Quiet, FALSE );
				break;

			case ID_RecogEnter:
				get( st_Recog, MUIA_String_Contents, &str );
				strcpy( cfg_RecogHex[Bactive-3], str );
				break;

			case ID_PatternEnter:
				get( st_Pattern, MUIA_String_Contents, &str );
				strcpy( cfg_RecogString[Bactive-3], str );
				break;

			case ID_CommandAEnter:
				get( st_CommandA, MUIA_String_Contents, &str );
				if(Bactive>2)
					strcpy( cfg_RecogCommand[Bactive-3], str );
				else
					strcpy( cfg_FileType[Bactive], str );
				break;

			case ID_CommandBEnter:
				get( st_CommandB, MUIA_String_Contents, &str );
				strcpy( cfg_RecogCommandB[Bactive-3], str );
				break;

			case ID_FiletypeChange:
				get( cy_Filetype, MUIA_Cycle_Active, &dummy );
				if ( dummy == 1 )
				{
					set( pp_CommandB, MUIA_Disabled, FALSE );
					get( st_CommandB, MUIA_String_Contents, &str );
					strcpy( cfg_RecogCommandB[Bactive-3], str );
					cfg_RecogType[Bactive-3] = TRUE;
				}
				else
				{
					set( pp_CommandB, MUIA_Disabled, TRUE );
					cfg_RecogType[Bactive-3] = FALSE;
				}
				break;
		}

		if ( signal && !global_QuitProgram ) Wait(signal);
	}

	if ( !global_KeyFile )
		AboutRequester();

	set( wi_Main, MUIA_Window_Open, FALSE );

	Fail();
}
