#include "Includes.h"
#include "Request.h"

/*
**
** StringRequester()
**
*/

char * StringRequester ( char *Title_String, char *Gadget_String, char *Reject_String, int MaxLength, int SkipGadget, BOOL *Skip, BOOL *Cancel )
{
	BOOL running = TRUE;
	ULONG signal;
	char * String, Result_String[512];

	*Skip = FALSE;
	*Cancel = FALSE;

	set( bt_StringSkip, MUIA_ShowMe, FALSE );
	set( bt_StringAll, MUIA_ShowMe, FALSE );
	if ( SkipGadget == 1 )
		set( bt_StringSkip, MUIA_ShowMe, TRUE );
	if ( SkipGadget == 2 )
		set( bt_StringAll, MUIA_ShowMe, TRUE );

	set( st_StringGadget, MUIA_String_Contents, Gadget_String );
	set( st_StringGadget, MUIA_String_BufferPos, strlen( Gadget_String ) );
	set( st_StringGadget, MUIA_String_Reject, Reject_String );
	set( st_StringGadget, MUIA_String_MaxLen, MaxLength );
	set( wi_String, MUIA_Window_ActiveObject, st_StringGadget );
	set( wi_String, MUIA_Window_Title, Title_String );

	set( wi_String, MUIA_Window_Open, TRUE );

	while( running )
	{
		switch( DoMethod( app_RumorOpus, MUIM_Application_Input, &signal ) )
		{
			case MUIV_Application_ReturnID_Quit:
				global_QuitProgram = QuitRequester();
				if( global_QuitProgram )
					running = FALSE;
				break;
			case ID_StringOkay:
				get( st_StringGadget, MUIA_String_Contents, &String );
				strcpy( Result_String, String );
				running = FALSE;
				break;
			case ID_StringSkip:
				running = FALSE;
				*Skip = TRUE;
				strcpy( Result_String, Gadget_String );
				break;
			case ID_StringAll:
				running = FALSE;
				*Skip = TRUE;
				get( st_StringGadget, MUIA_String_Contents, &String );
				strcpy( Result_String, String );
				break;
			case ID_StringCancel:
				running = FALSE;
				*Cancel = TRUE;
				strcpy( Result_String, "" );
				break;
		}
		if( signal ) Wait( signal );
	}

	set( wi_String, MUIA_Window_Open, FALSE );
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

	if ( global_Actions > 29 )
		global_Actions = 0;

	if ( !global_Special )
	{
		if ( !global_KeyFile )
		{
			strcpy( About_String, "" );
			strcat( About_String, "\33c\33uRO 1.28 Beta (19.06.18)\n\33n" );
			strcat( About_String, "Copyright © 1994-2018 by Oliver Rummeyer\n\n" );
			strcat( About_String, "\33b- Unregistered Release -\33n\n\n" );
			strcat( About_String, "To register, send $20.- or DM 30.- to\n\n" );
			strcat( About_String, "Oliver Rummeyer\n" );
			strcat( About_String, "Kreuzäckerstr. 31\n" );
			strcat( About_String, "70794 Filderstadt\n" );
			strcat( About_String, "Germany/Europe\n\n" );
			strcat( About_String, "e-mail: oliver@rummeyer.de\n\n" );
			strcat( About_String, GetCatStr( 4, "This is a MUI-Application\n" ) );
			strcat( About_String, "MUI is copyrighted by Stefan Stuntz" );
		}
		else
		{
			strcpy( About_String, "" );
			strcat( About_String, "\33c\33uRO 1.28 Beta (19.06.18)\n\33n" );
			strcat( About_String, "Copyright © 1994-2018 by Oliver Rummeyer\n\n" );
			strcat( About_String, "\33b- Registered Release -\33n\n\n" );
			strcat( About_String, "Registered to\n" );
			strcat( About_String, global_Username );
			strcat( About_String, "\n(SerialNr " );
			strcat( About_String, global_Userserial );
			strcat( About_String, ")\n\n");
			strcat( About_String, GetCatStr( 4, "This is a MUI-Application\n" ) );
			strcat( About_String, "MUI is copyrighted by Stefan Stuntz" );
		}
	}
	else
	{
		strcpy( About_String, "" );
		strcat( About_String, "\33c\33uRO 1.28 Beta (19.06.18)\n\33n" );
		strcat( About_String, "Copyright © 1994-2018 by Oliver Rummeyer\n\n" );
		strcat( About_String, "\33b- Free Edition -\33n\n\n" );
		strcat( About_String, GetCatStr( 4, "This is a MUI-Application\n" ) );
		strcat( About_String, "MUI is copyrighted by Stefan Stuntz" );
	}
	MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 3, "About RO" ), GetCatStr( 72, "_Okay" ), About_String, TAG_END );

	return( TRUE );
}
