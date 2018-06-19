#include "Includes.h"
#include "Hooks.h"

/*
**
** AppMsgFunc()
**
*/

SAVEDS ASM LONG AppMsgFunc( REG(a2) APTR obj, REG(a1) struct AppMessage **x )
{
	struct WBArg *ap;
	struct AppMessage *amsg = *x;
	char Buffer[512], NewBuffer[512], path[512];
	LONG Over, DelForce;
	int i, side = -1, ErrorNum = 0;
	BPTR newlock;
	ULONG iconified = 0;

	Sleep( TRUE );

	set( ga_Gauge, MUIA_Gauge_Current, 0 );
	if ( cfg_DropIcons )
	{
		Over = cfg_Overwrite;
		DelForce = cfg_DelForce;
		set( wi_Progress, MUIA_Window_Open, TRUE );
	}

	for ( ap = amsg -> am_ArgList, i = 0; i < amsg -> am_NumArgs; i++, ap++ )
	{
		NameFromLock( ap -> wa_Lock, Buffer, sizeof( Buffer ) );
		newlock = Lock( Buffer, ACCESS_READ );
		if ( newlock )
		{
			NameFromLock( newlock, NewBuffer, sizeof( NewBuffer ) );
			if ( stricmp( NewBuffer, Buffer ) != 0 )
				ErrorNum = -11;
			UnLock( newlock );
		}
		else
			ErrorNum = -11;

		if ( ErrorNum == 0 )
		{
			get( app_RumorOpus, MUIA_Application_Iconified, &iconified );
			if ( !iconified )
			{
				if ( obj == pg_Page[Left_Side] ) side = Left_Side;
				if ( obj == pg_Page[Right_Side] ) side = Right_Side;
				if ( cfg_DropIcons )
				{
					if ( ( side != -1 ) && global_DirLoaded[side] )
					{
						if ( i == 0 )
							set( ga_Gauge, MUIA_Gauge_Max, amsg -> am_NumArgs );

						set( pg_Page[side], MUIA_Group_ActivePage, 0 );
						strcpy( path, GetPath( side ) );
						if (  strlen( ap -> wa_Name ) > 0 )
						{
							AddPart( Buffer, ap -> wa_Name, sizeof( Buffer ) );
							AddPart( path, ap -> wa_Name, sizeof( path ));
							ErrorNum = CopyFile( Buffer, path, TRUE );
							if ( ErrorNum == -20 )
								ErrorNum = 0;
							if ( ErrorNum == 0 )
								CopyFile( strcat( Buffer, ".info" ), strcat( path, ".info" ), TRUE );
						}
						else
						{
							AddPart( path, FilePart( Buffer ), sizeof( path ) );
							if ( strstr( path, Buffer ) == NULL )
							{
								ErrorNum = CopyDirectory( Buffer, path );
								if ( ErrorNum == -20 )
									ErrorNum = 0;
								if ( ErrorNum == 0 )
								{
									Clone( Buffer, path );
									CopyFile( strcat( Buffer, ".info" ), strcat( path, ".info" ), TRUE );
								}
							}
							else
								ErrorNum = -5;
						}

						set( ga_Gauge, MUIA_Gauge_Current, i+1 );
					}
				}
				else
					if ( side != -1 )
						ErrorNum = LoadDirectory( Buffer, side );
			}
			else
			{
				ErrorNum = LoadDirectory( Buffer, Active_Side );
				Delay( 25 );
				if ( ErrorNum == 0 )
					if ( strlen( ap -> wa_Name ) > 0 )
					{
						AddPart( Buffer, ap -> wa_Name, sizeof( Buffer ) );
						ErrorNum = FileAction( Buffer );
					}
			}
		}
		if ( ErrorNum != 0 )
			break;
	}

	if ( cfg_DropIcons )
	{
		set( wi_Progress, MUIA_Window_Open, FALSE );
		cfg_Overwrite = Over;
		cfg_DelForce = DelForce;
		if ( ErrorNum == 0 )
			Reload( side );
	}

	if ( ErrorNum != 0 )
	{
		strcpy( Status_String, strdup( Error( ErrorNum ) ) );
		set( bt_StatusBar, MUIA_Text_Contents, Status_String );

		if ( iconified )
			DisplayBeep( 0 );
	}
	else
		SleepClock( FALSE );

	SleepWindow ( FALSE );

	return( 0 );
}

/*
**
** ButtonFunc()
**
*/

SAVEDS ASM LONG ButtonFunc( REG(a2) APTR obj )
{
	int i;

	SleepClock( FALSE );

	for ( i = 0; i < 120; i++ )
		if ( obj == bt_Button[i] )
			break;

	Button( i );

	return( 0 );
}

/*
**
** DriveFunc()
**
*/

SAVEDS ASM LONG DriveFunc( REG(a2) APTR obj )
{
	int i, side;

	SleepClock( FALSE );

	for ( i = 0; i < 32; i++ )
		if ( obj == bt_Drive[i] )
			break;

	if ( cfg_DriveButtons )
		side = Active_Side;
	else
		if ( i < 8 || ( i > 15 && i < 24 ) ) side = Left_Side;
		else side = Right_Side;
		
	LoadDirectory( cfg_Drive[i], side );

	return( 0 );
}

/*
**
** ActionRexxFunc()
**
*/

SAVEDS ASM LONG ActionRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr, String[256];

	if ( cptr = (char *)*arg )
	{
		strcpy( String, cptr );
		FileAction( String );
	}

	return( RETURN_OK );
}

/*
**
** ActiveRexxFunc()
**
*/

SAVEDS ASM LONG ActivateRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr, String[10];
	int side;

	switch( Active_Side )
	{
		case Left_Side:
			strcpy( String, "left" );
			break;
		case Right_Side:
			strcpy( String, "right" );
			break;
	}

	if ( cptr = (char *)*arg )
	{
		side = StringSide ( cptr );
		if ( side != -1 )
		{
			ActivateList( side );
		}
	}

	set( app_RumorOpus, MUIA_Application_RexxString, String );

	return( RETURN_OK );
}

/*
**
** LoadRexxFunc()
**
*/

SAVEDS ASM LONG LoadRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr, String[256];
	int side;

	if ( cptr = (char *)*arg )
	{
		side = StringSide ( cptr );
		if ( side != -1 )
		{
			arg++;
			if ( cptr = (char *)*arg )
			{
				strcpy( String, cptr );
				LoadDirectory( String, side );
			}
		}
	}

	return( RETURN_OK );
}

/*
**
** PathRexxFunc()
**
*/

SAVEDS ASM LONG PathRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr, String[256];
	int side;

	if ( cptr = (char *)*arg )
	{
		side = StringSide( cptr );
		if ( side != -1 )
		{
			strcpy( String, "" );
			if ( global_DirLoaded[side] )
				strcpy( String, GetPath( side ) );
			if ( strlen( String ) > 0 )
				set( app_RumorOpus, MUIA_Application_RexxString, String );
		}
	}

	return( RETURN_OK );
}

/*
**
** RefreshRexxFunc()
**
*/

SAVEDS ASM LONG RefreshRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr;
	int side;

	if ( cptr = (char *)*arg )
	{
		side = StringSide (cptr );
		if ( side != -1 )
			Reload( side );
	}

	return( RETURN_OK );
}

/*
**
** MessageRexxFunc()
**
*/

SAVEDS ASM LONG MessageRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr;

	if ( cptr = (char *)*arg )
	{
		strcpy( Status_String, cptr );
		set( bt_StatusBar, MUIA_Text_Contents, Status_String );
	}

	return( RETURN_OK );
}

/*
**
** ChoiceRexxFunc()
**
*/

SAVEDS ASM LONG ChoiceRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr, String[2][256];
	int rc;

	if ( cptr = (char *)*arg )
	{
		strcpy( String[0], cptr );
		strcpy( String[1], "*" );
		strcat( String[1], GetCatStr( 72, "_Okay" ) );
		strcat( String[1], "|" );
		strcat( String[1], GetCatStr( 73, "_Cancel" ) );
		rc = MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 50, "Request" ), String[1], String[0], TAG_DONE );
		sprintf( String[0], "%d", rc );
		set( app_RumorOpus, MUIA_Application_RexxString, String[0] );
	}

	return( RETURN_OK );
}

/*
**
** StringRexxFunc()
**
*/

SAVEDS ASM LONG StringRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr, String[256];
	BOOL Skip, Cancel;

	if ( cptr = (char *)*arg )
	{
		strcpy( String, cptr );
		strcpy( String, StringRequester( GetCatStr( 75, "Input Request" ), String, "", 256, 0, &Skip, &Cancel ) );
		if ( !Cancel )
			set( app_RumorOpus, MUIA_Application_RexxString, String );
	}

	return( RETURN_OK );
}

/*
**
** NumEntriesRexxFunc()
**
*/

SAVEDS ASM LONG NumEntriesRexxFunc( REG(a1) ULONG *arg )
{
	int side, i, j=0;
	ULONG Entries_ULONG, Selection_State, Type_ULONG = 0;
	char *cptr, String[81], Type_String[10];
	__aligned struct FileInfoBlock * fib;

	if ( cptr = (char *)*arg )
	{
		side = StringSide( cptr );
		if ( side != -1 )
		{
			arg++;
			if ( cptr = (char *)*arg )
			{
				strcpy( Type_String, cptr );
				if ( stricmp( Type_String, "FILES" ) == 0 )
					Type_ULONG = 1;
				if ( stricmp( Type_String, "DIRS" ) == 0 )
					Type_ULONG = 2;

				get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );
				for ( i = 0; i < Entries_ULONG; i++ )
				{
					DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
					if ( Selection_State == MUIV_List_Select_On )
					{
						DoMethod( lv_Directory[side], MUIM_List_GetEntry, i, &fib );
						if ( ( Type_ULONG == 0 ) || ( ( Type_ULONG == 1 ) && ( fib -> fib_DirEntryType < 0 ) ) || ( ( Type_ULONG == 2 ) && ( fib -> fib_DirEntryType > 0 ) ) )
							j++;
					}
				}
				sprintf( String, "%ld", j );
				set( app_RumorOpus, MUIA_Application_RexxString, String );
			}
		}
	}

	return( RETURN_OK );
}

/*
**
** GetEntryRexxFunc()
**
*/

SAVEDS ASM LONG GetEntryRexxFunc( REG(a1) ULONG *arg )
{
	int side;
	char String[512], *cptr, Type_String[10];
	ULONG i, Entries_ULONG, Selection_State, Type_ULONG = 0;
	__aligned struct FileInfoBlock * fib;

	if ( cptr = (char *)*arg )
	{
		side = StringSide( cptr );
		if ( side != -1 )
		{
			arg++;
			if ( cptr = (char *)*arg )
			{
				strcpy( Type_String, cptr );
				if ( stricmp( Type_String, "FILES" ) == 0 )
					Type_ULONG = 1;
				if ( stricmp( Type_String, "DIRS" ) == 0 )
					Type_ULONG = 2;

				strcpy( String, "" );
				get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );
				for ( i = 0; i < Entries_ULONG; i++ )
				{
					DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
					if ( Selection_State == MUIV_List_Select_On )
					{
						DoMethod( lv_Directory[side], MUIM_List_GetEntry, i, &fib );
						if ( ( Type_ULONG == 0 ) || ( ( Type_ULONG == 1 ) && ( fib -> fib_DirEntryType < 0 ) ) || ( ( Type_ULONG == 2 ) && ( fib -> fib_DirEntryType > 0 ) ) )
						{
							sprintf( String, "%s%s", GetPath( side ), fib -> fib_FileName );
							DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Off, NULL );
							break;
						}
					}
				}
			}
			UpdateNumFiles( side );
		}
		if ( strlen ( String ) > 0 )
			set( app_RumorOpus, MUIA_Application_RexxString, String );
	}

	return( RETURN_OK );
}

/*
**
** CommandRexxFunc()
**
*/

SAVEDS ASM LONG CommandRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr, String[2][512];
	int ErrorNum = 0;

	if ( cptr = (char *)*arg )
	{
		strcpy( String[0], cptr );
		arg++;
		if ( cptr = (char *)*arg )
		{
			strcpy( String[1], cptr );
			ErrorNum = ExecuteCommand ( String[0], String[1] );
		}
	}

	global_AskOnce = FALSE;

	if ( ErrorNum != 0 )
		return( RETURN_ERROR );
	else
		return( RETURN_OK );
}

/*
**
** FunctionRexxFunc()
**
*/

SAVEDS ASM LONG FunctionRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr;

	if ( cptr = (char *)*arg )
	{
		strcpy( cfg_ButtonCommand[170], cptr );
		cfg_ButtonType[170] = TRUE;
		Button(170);
	}

	return( RETURN_OK );
}

/*
**
** ChangeRexxFunc()
**
*/

SAVEDS ASM LONG ChangeRexxFunc( REG(a1) ULONG *arg )
{
	char *cptr, String[256];
	int side;

	if ( cptr = (char *)*arg )
	{
		side = StringSide ( cptr );
		if ( side != -1 )
		{
			arg++;
			if ( cptr = (char *)*arg )
			{
				strcpy( String, cptr );
				if(stricmp("Dirs",String)==0) cfg_FirstType[side]=MUIV_Dirlist_SortDirs_First;
				if(stricmp("Files",String)==0) cfg_FirstType[side]=MUIV_Dirlist_SortDirs_Last;
				if(stricmp("Mixed",String)==0) cfg_FirstType[side]=MUIV_Dirlist_SortDirs_Mix;
				if(stricmp("Name",String)==0) cfg_SortType[side]=MUIV_Dirlist_SortType_Name;
				if(stricmp("Date",String)==0) cfg_SortType[side]=MUIV_Dirlist_SortType_Date;
				if(stricmp("Size",String)==0) cfg_SortType[side]=MUIV_Dirlist_SortType_Size;

				if(stricmp("High",String)==0) cfg_SortHighLow[side]=FALSE;
				if(stricmp("Low",String)==0) cfg_SortHighLow[side]=TRUE;

				set( lv_Directory[side], MUIA_Dirlist_SortType, cfg_SortType[side] );
				set( lv_Directory[side], MUIA_Dirlist_SortDirs, cfg_FirstType[side] );
				set( lv_Directory[side], MUIA_Dirlist_SortHighLow, cfg_SortHighLow[side] );
			}
		}
	}

	return( RETURN_OK );
}
