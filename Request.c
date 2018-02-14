#include "Includes.h"
#include "Request.h"

/*
**
** StringRequester()
**
*/

char * StringRequester ( char *Title_String, char *Gadget_String, char *Reject_String, int MaxLength, BOOL SkipGadget, BOOL *Skip, BOOL *Cancel )
{
	BOOL running = TRUE;
	ULONG signal;
	APTR wi_String, st_String, bt_Okay, bt_Cancel, bt_Skip;
	char * String, Result_String[512];

	*Skip = FALSE;
	*Cancel = FALSE;

	wi_String = WindowObject,
		MUIA_Window_ID, 3,
		MUIA_Window_Title, Title_String,
		MUIA_Window_Menu, MUIV_Window_Menu_NoMenu,
		WindowContents, VGroup,
			Child, st_String = StringObject,
				StringFrame,
				MUIA_String_Contents, Gadget_String,
				MUIA_String_BufferPos, strlen( Gadget_String ),
				MUIA_String_Reject, Reject_String,
				MUIA_String_MaxLen, MaxLength,
				End,
			Child, HGroup,
				MUIA_Group_SameSize, TRUE,
				Child, bt_Okay = SimpleButton( GetCatStr( 72, "_Okay" ) ),
				Child, HSpace(0),
				Child, bt_Skip = SimpleButton( GetCatStr( 119, "_Skip" ) ),
				Child, HSpace(0),
				Child, bt_Cancel = SimpleButton( GetCatStr( 73, "_Cancel" ) ),
				End,
			End,
		End;

	if( !wi_String || !CheckMemory( 50000 ) )
		Fail();

	DoMethod( wi_String, MUIM_Window_SetCycleChain, st_String, bt_Okay, bt_Skip, bt_Cancel, NULL );
	DoMethod( wi_String, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, 3 );
	DoMethod( st_String, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpus, 2, MUIM_Application_ReturnID, 1 );
	DoMethod( bt_Okay, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 1 );
	DoMethod( bt_Skip, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2 );
	DoMethod( bt_Cancel, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 3 );

	DoMethod( app_RumorOpus, OM_ADDMEMBER, wi_String );
	set( wi_String, MUIA_Window_Open, TRUE );
	set( wi_String, MUIA_Window_ActiveObject, st_String );

	while( running )
	{
		switch( DoMethod( app_RumorOpus, MUIM_Application_Input, &signal ) )
		{
			case MUIV_Application_ReturnID_Quit:
				running = FALSE;
				global_QuitProgram = QuitRequester();
				break;
			case 1:
				get( st_String, MUIA_String_Contents, &String );
				strcpy( Result_String, String );
				running = FALSE;
				break;
			case 2:
				running = FALSE;
				*Skip = TRUE;
				strcpy( Result_String, Gadget_String );
				break;
			case 3:
				running = FALSE;
				*Cancel = TRUE;
				strcpy( Result_String, "" );
				break;
		}
		if( signal ) Wait( signal );
	}
	set( wi_String, MUIA_Window_Open, FALSE );
	DoMethod( app_RumorOpus, OM_REMMEMBER, wi_String );
	MUI_DisposeObject( wi_String );
	return( Result_String );
}

/*
**
** QuitRequester()
**
*/

BOOL QuitRequester ( void )
{
	BOOL Quit;
	LONG Force;
	int num;

	get( app_RumorOpus, MUIA_Application_ForceQuit, &Force );
	if( !Force && cfg_QuitVerify )
	{
		Sleep( TRUE );
		num = MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 50, "Request" ), GetCatStr( 117, "*_Okay|_Iconify|_Cancel" ), GetCatStr( 118, "Do you really want to quit 'RO'?" ), TAG_END );
		switch( num )
		{
			case 1:
				Quit = TRUE;
				break;
			case 2:
				set( app_RumorOpus, MUIA_Application_Iconified, TRUE );
				Quit = FALSE;
				break;
			case 0:
				Quit = FALSE;
				break;
		}
		Sleep( FALSE );
		return( Quit );
	}
	return( TRUE );
}

/*
**
** AboutRequester()
**
*/

BOOL AboutRequester ( void )
{
	char About_String[512];

	if ( global_Actions > 9 )
		global_Actions = 0;

	if ( !global_KeyFile )
	{
		strcpy( About_String, "" );
		strcat( About_String, "\33c\33uRO 1.20 (3.12.95)\n\33n" );
		strcat( About_String, "Copyright © 1994, 1995 by Oliver Rummeyer\n\n" );
		strcat( About_String, "\33b- Unregistered Release -\33n\n\n" );
		strcat( About_String, "To register, send $20.- or DM 30.- to\n\n" );
		strcat( About_String, "Oliver Rummeyer\n" );
		strcat( About_String, "Hochbergstraﬂe 49/1\n" );
		strcat( About_String, "D-88213 Ravensburg\n" );
		strcat( About_String, "Germany/Europe\n\n" );
		strcat( About_String, "e-mail: oliver.rummeyer@student.uni-ulm.de\n\n" );
		strcat( About_String, GetCatStr( 4, "This is a MUI-Application\n" ) );
		strcat( About_String, "MUI is copyrighted by Stefan Stuntz" );
	}
	else
	{
		strcpy( About_String, "" );
		strcat( About_String, "\33c\33uRO 1.20 (3.12.95)\n\33n" );
		strcat( About_String, "Copyright © 1994, 1995 by Oliver Rummeyer\n\n" );
		strcat( About_String, "\33b- Registered Release -\33n\n\n" );
		strcat( About_String, "Registered to\n" );
		strcat( About_String, global_Username );
		strcat( About_String, "\n(SerialNr " );
		strcat( About_String, global_Userserial );
		strcat( About_String, ")\n\n");
		strcat( About_String, GetCatStr( 4, "This is a MUI-Application\n" ) );
		strcat( About_String, "MUI is copyrighted by Stefan Stuntz" );
	}
	MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 3, "About RO" ), GetCatStr( 72, "_Okay" ), About_String, TAG_END );

	return( TRUE );
}
