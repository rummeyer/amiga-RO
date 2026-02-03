#include "Includes.h"
#include "Dir.h"

/*
**
** Reload()
**
*/

void Reload ( int side )
{
	if ( global_DirLoaded[side] )
	{
		global_Reload = TRUE;
		global_CursorPos[side] = MUIV_List_Active_Off;
		get( lv_Directory[side], MUIA_List_Active, &global_CursorPos[side] );
		LoadDirectory( global_Path[side], side );
	}
	if ( stricmp( GetPath( Left_Side ), GetPath( Right_Side ) ) == 0 )
	{
		if ( global_DirLoaded[OtherSide( side )] )
		{
			global_Reload = TRUE;
			global_CursorPos[OtherSide( side )] = MUIV_List_Active_Off;
			get( lv_Directory[OtherSide( side )], MUIA_List_Active, &global_CursorPos[OtherSide( side )] );
			LoadDirectory( global_Path[OtherSide( side )], OtherSide( side ) );
		}
	}
}

/*
**
** LoadDirectory()
**
*/

int LoadDirectory ( char * Path_String, int side )
{
	__aligned struct FileInfoBlock * fib;
	BPTR lock, newlock, oldlock;
	APTR ActiveObject_APTR;
	char NewPath_String[4096], Buffer[4096], * Buffer_String;
	LONG Entries_LONG, i;
	BOOL Found_BOOL = FALSE, success;
	int ErrorNum = 0;

	fib = malloc( sizeof ( struct FileInfoBlock ) );
	if ( fib )
	{
		lock = Lock( Path_String, ACCESS_READ );
		if ( lock )
		{
			oldlock = CurrentDir( lock );
			lock = CurrentDir( oldlock );

			if ( !global_Reload )
				global_CursorPos[side] = MUIV_List_Active_Off;
			else
				global_Reload = FALSE;

			if ( !global_KeyFile )
				global_Actions++;
			Examine( lock, fib );
			if ( fib -> fib_DirEntryType > 0 )
			{
				get( wi_Main, MUIA_Window_ActiveObject, &ActiveObject_APTR );
				set( bt_StatusText[side], MUIA_Text_Contents, GetCatStr( 41, "Reading..." ) );

				if ( cfg_PathExpand )
				{
					success = NameFromLock( lock, NewPath_String, sizeof( NewPath_String ) );
					if ( success && ( newlock = Lock( NewPath_String, ACCESS_READ ) ) )
					{
						success = NameFromLock( newlock, Buffer, sizeof( Buffer ) );
						if ( success )
						{
							if ( stricmp( NewPath_String, Buffer ) != 0 )
								strcpy( NewPath_String, Path_String );
						}
						UnLock( newlock );
					}
					else
						strcpy( NewPath_String, Path_String );
				}
				else
					strcpy( NewPath_String, Path_String );

				if ( ErrorNum == 0 )
				{
					AddPart( NewPath_String, "", sizeof( NewPath_String ) );
					if ( stricmp( NewPath_String, ":" ) == 0 )
						strcpy( NewPath_String, Path_String );

					set( lv_Directory[side], MUIA_Dirlist_Directory, NewPath_String );
					set( st_PathGadget[side], MUIA_String_Contents, NewPath_String );
					strcpy( global_Path[side], NewPath_String);

					get( lv_Buffers[Left_Side], MUIA_List_Entries, &Entries_LONG );

					for ( i = 0; i < Entries_LONG; i++ )
					{
						DoMethod( lv_Buffers[Left_Side], MUIM_List_GetEntry, i, &Buffer_String );
						if ( stricmp( NewPath_String, Buffer_String ) == 0 )
						{
							Found_BOOL = TRUE;
							break;
						}
					}

					if ( !Found_BOOL || !cfg_CheckDupes )
					{
						if ( Entries_LONG != cfg_History )
							for ( i = 0; i < 2; i++ )
								DoMethod( lv_Buffers[i], MUIM_List_InsertSingle, NewPath_String, global_NumBuffers );
						else
							for ( i = 0; i < 2; i++ )
							{
								DoMethod( lv_Buffers[i], MUIM_List_Remove, MUIV_List_Remove_Last );
								DoMethod( lv_Buffers[i], MUIM_List_InsertSingle, NewPath_String, global_NumBuffers );
							}
					}

					DoMethod( lv_Directory[side], MUIM_Dirlist_ReRead );
					set( pg_Page[side], MUIA_Group_ActivePage, 0 );
				}
			}
			UnLock( lock );
		}
		else
			ErrorNum = IoErr();
		free( fib );
	}
	else
		ErrorNum = ERR_NO_MEMORY;

	return( ErrorNum );
}

/*
** LoadParent() - Navigate up one directory level
** Strips the last path component and loads the parent directory
*/

void LoadParent ( int side )
{
	char String[4096], NewPath_String[4096], *cptr;

	if ( !global_DirLoaded[side] )
	{
		set( pg_Page[side], MUIA_Group_ActivePage, 2 );
		return;
	}

	strcpy( String, GetPath( side ) );
	cptr = strrchr( String, '/' );
	if ( cptr )
	{
		cptr++;
		if ( *cptr == '\0' )
		{
			cptr--;
			*cptr = '\0';
		}
		strmid( String, NewPath_String, 1, strlen( String ) - strlen( FilePart( String ) ) );
		LoadDirectory( NewPath_String, side );
	}
	else
		set( pg_Page[side], MUIA_Group_ActivePage, 2 );
}

/*
** LoadRoot() - Navigate to the root directory of current volume
** Extracts device name (up to ':') and loads its root
*/

void LoadRoot ( int side )
{
	char Path_String[4096];
	char * cptr;

	if ( global_DirLoaded[side] )
	{
		strcpy( Path_String, GetPath( side ) );
		cptr = strrchr( Path_String, ':' );
		if ( cptr )
		{
			cptr++;
			*cptr = '\0';
		}
		LoadDirectory( Path_String, side );
	}
}

/*
**
** LoadVolume()
**
*/

void LoadVolume ( int side )
{
	char * Volume_String;

	DoMethod( lv_Volumes[side], MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &Volume_String );
	LoadDirectory( Volume_String, side );
}

/*
**
** LoadBuffer()
**
*/

void LoadBuffer ( int side )
{
	char * Buffer_String, * Help_String, Path[4096];
	int i, len;

	DoMethod( lv_Buffers[side], MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &Buffer_String );
	Help_String = strstr( Buffer_String, "\33b" );
	if ( Help_String != NULL )
	{
		len = strlen( Buffer_String );
		for( i = 2; i < len; i++ )
		{
			Path[i-2] = Buffer_String[i];
		}
		Path[i-2] = 0;
		LoadDirectory( Path, side );
	}
	else
		LoadDirectory( Buffer_String, side );
}

/*
**
** UpdateStatusText()
**
*/

void UpdateStatusText ( int side )
{
	char Device_String[512], Text_String[128], Unit_Free[2], Unit_Used[3];
	char BytesFree_String[11], BytesUsed_String[11];
	char * cptr;
	BPTR lock, newlock;
	struct InfoData * pid;
	ULONG KBytesTotal_ULONG, KBytesUsed_ULONG, KBytes_Used, KBytes_Free;
	LONG Entries_ULONG, Visible_ULONG;
	BOOL success;
	int i,j,k=0,Percent = 100;
	struct DateStamp * current;

	lock = Lock( GetPath( side ), ACCESS_READ );
	if ( lock )
	{
		success = NameFromLock( lock, Device_String, sizeof( Device_String ) );
		if ( success && ( newlock = Lock( Device_String, ACCESS_READ ) ) )
		{
			cptr = strrchr( Device_String, ':' );
			* cptr = '\0';
			UnLock( newlock );
		}
		else
			strcpy( Device_String, "" );

		pid = malloc( sizeof( struct InfoData ) );
		if ( pid )
		{
			Info(lock,pid);
			KBytesTotal_ULONG = (( pid -> id_NumBlocks ) / 1024) * ( pid -> id_BytesPerBlock );
			KBytesUsed_ULONG = (( pid -> id_NumBlocksUsed) / 1024) * ( pid -> id_BytesPerBlock );
			if (KBytesTotal_ULONG < ( 1024 * 1024 ))
				KBytesTotal_ULONG = ( pid -> id_NumBlocks ) * ( pid -> id_BytesPerBlock ) / 1024;
			if (KBytesUsed_ULONG < ( 1024 * 1024 ))
				KBytesUsed_ULONG = ( pid -> id_NumBlocksUsed) * ( pid -> id_BytesPerBlock ) / 1024;

			if ( ( KBytesTotal_ULONG / 100 ) > 0 )
				Percent = ( KBytesUsed_ULONG / ( KBytesTotal_ULONG / 100 ) );
			if ( Percent > 100 ) Percent = 100;

			strcpy( Unit_Free, "K" );
			strcpy( Unit_Used, "K" );
			KBytes_Free = ( KBytesTotal_ULONG - KBytesUsed_ULONG );
			KBytes_Used = KBytesUsed_ULONG;
			if ( KBytes_Free > 9999 )
			{
				KBytes_Free = KBytes_Free / 1024;
				strcpy( Unit_Free, "M" );
			}
			if ( KBytes_Used > 9999 )
			{
				KBytes_Used = KBytes_Used / 1024;
				strcpy( Unit_Used, "M" );
			}
			strcpy( BytesFree_String, NumberToString( KBytes_Free ) );
			strcpy( BytesUsed_String, NumberToString( KBytes_Used ) );
			sprintf( Text_String, GetCatStr( 42, "%s  %ld%% full, %s%s free, %s%s in use" ), Device_String, Percent, BytesFree_String, Unit_Free, BytesUsed_String, Unit_Used );
			set( bt_StatusText[side], MUIA_Text_Contents, Text_String );
			if ( !global_DirLoaded[side] ) global_DirLoaded[side] = TRUE;
			if ( !global_KeyFile )
			{
				if ( global_Actions > 29 )
				{
					SleepWindow( TRUE );
					current = malloc( sizeof ( struct DateStamp ) );
					if ( current )
					{
						DateStamp( current );
						i = current->ds_Tick;
						j = i;
						while ( ( j >= i && ( j - i < 256 ) ) && AboutRequester() )
						{
							k++;
							DateStamp( current );
							j = current->ds_Tick;
						}
						if ( global_Actions != 0 || k > 25 ) Fail();
						free( current );
					}
					SleepWindow( FALSE );
				}
			}
			else
				if ( global_Actions != 0 )
					Fail();
			free( pid );
		}
		UnLock( lock );
	}

	if ( global_CursorPos[side] != MUIV_List_Active_Off )
	{
		get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );
		get( lv_Directory[side], MUIA_List_Visible, &Visible_ULONG );
		if ( ( Entries_ULONG - 1 ) < global_CursorPos[side] )
			global_CursorPos[side] = Entries_ULONG - 1;
		if ( Visible_ULONG > 0 )
			if ( ( Entries_ULONG - 1 ) > global_CursorPos[side] + ( Visible_ULONG / 2 ) )
				DoMethod( lv_Directory[side], MUIM_List_Jump, global_CursorPos[side] + ( Visible_ULONG / 2 ) );
			else
				DoMethod( lv_Directory[side], MUIM_List_Jump, Entries_ULONG - 1 );
		set( lv_Directory[side], MUIA_List_Active, global_CursorPos[side] );
		global_CursorPos[side] = MUIV_List_Active_Off;
	}

}

/*
**
** UpdateNumFiles()
**
*/

void UpdateNumFiles( int side )
{
	char Text_String[128], NumFiles_String[41], NumDirs_String[41], NumBytes_String[41];
	ULONG i, NumFiles = 0, NumDirs = 0, NumBytes = 0, Size, Entries_ULONG, Selection_State;
	__aligned struct FileInfoBlock * fib;

	if ( global_DirLoaded[side] )
	{
		get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );
		for ( i = 0; i < Entries_ULONG; i++ )
		{
			DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
			DoMethod( lv_Directory[side], MUIM_List_GetEntry, i, &fib );
			if ( Selection_State == MUIV_List_Select_On )
			{
				Size = fib -> fib_Size;
				if ( fib -> fib_DirEntryType < 0 )
				{
					NumBytes = NumBytes + Size;
					NumFiles++;
				}
				else
					NumDirs++;
			}
		}

		if ( ( NumDirs > 0 ) || ( NumFiles > 0 ) )
		{
			strcpy( NumFiles_String, NumberToString( NumFiles ) );
			strcpy( NumDirs_String, NumberToString( NumDirs ) );
			strcpy( NumBytes_String, NumberToString( NumBytes ) );
			sprintf( Text_String, GetCatStr( 121, "%s file(s), %s drawer(s), %s bytes" ), NumFiles_String, NumDirs_String, NumBytes_String );
			set( bt_StatusText[side], MUIA_Text_Contents, Text_String );
		}
		else
			UpdateStatusText( side );
	}
}
