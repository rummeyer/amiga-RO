#include "Includes.h"
#include "Functions.h"

/*
**
** ListSwap()
**
*/

void ListSwap ( void )
{
	char Path_String[512];
	if ( global_DirLoaded[Left_Side] && global_DirLoaded[Right_Side] )
	{
		strcpy( Path_String, GetPath( Left_Side ) );
		LoadDirectory( GetPath( Right_Side ), Left_Side );
		LoadDirectory( Path_String, Right_Side );
	}
}

/*
**
** ListCopy()
**
*/

void ListCopy ( int side )
{
	if ( global_DirLoaded[side] ) LoadDirectory( GetPath( side ), OtherSide( side ));
}

/*
**
** ListFold()
**
*/

void ListFold ( void )
{
	global_Unfolded = !global_Unfolded;
	DoMethod( FoldGroup, MUIM_Group_InitChange );
	set( bl_Balance, MUIA_ShowMe, global_Unfolded );
	set( pg_Page[Right_Side], MUIA_ShowMe, global_Unfolded );
	ActivateList(Left_Side);
	ActivateList(Left_Side);
	DoMethod( FoldGroup, MUIM_Group_ExitChange );
}

/*
**
** CreateDirectory()
**
*/

int CreateDirectory ( int side, BOOL LoadIt )
{
	BPTR lock;
	BOOL Skip, Cancel;
	char Path_String[512], Icon_String[512], * String;
	int ErrorNum = 0;

	if ( global_DirLoaded[side] )
	{
		String = StringRequester( GetCatStr( 94, "Enter directory name" ), "", ":/", 31, 0, &Skip, &Cancel );
		if ( !Cancel && strlen( String ) > 0 )
		{
			strcpy( Path_String, GetPath( side ) );
			AddPart( Path_String, String, sizeof( Path_String ) );
			lock = CreateDir( Path_String );
			if ( lock )
			{
				UnLock( lock );
				if ( cfg_CreateIcons && ( ( strlen( FilePart( Path_String ) ) + 5 ) < 31 ) )
				{
					sprintf( Icon_String, "%s.info", Path_String );
					ErrorNum = CopyFile( "ENV:sys/def_drawer.info", Icon_String, FALSE );
				}
				if ( LoadIt )
					LoadDirectory( Path_String, side );
				else
					Reload( side );
			}
			else
				ErrorNum = IoErr();
		}
	}
	return( ErrorNum );
}

/*
**
** RelabelDevice()
**
*/

int RelabelDevice ( int side )
{
	BOOL Skip, Cancel;
	BOOL success;
	BPTR lock;
	char * NewName_String, * cptr;
	char Title_String[61], OldName_String[32], String[512];
	int ErrorNum = 0;

	if ( global_DirLoaded[side] )
	{
		lock = Lock ( GetPath( side ), ACCESS_READ );
		if ( lock )
		{
			success = NameFromLock( lock, String, sizeof( String ) );
			if ( !success )
				ErrorNum = IoErr();
			else
				if ( stricmp( String, ":" ) == 0 )
					ErrorNum = -11;

			if ( ErrorNum == 0 )
			{
				cptr = strrchr( String, ':' );
				* cptr = '\0';
				strcpy( OldName_String, String );
				sprintf( Title_String, GetCatStr( 95, "Relabel '%s' as" ), OldName_String );
				NewName_String = StringRequester( Title_String, OldName_String, ":/", 31, 0, &Skip, &Cancel );
				strcat( OldName_String, ":" );
				if ( !Cancel )
				{
					if ( stricmp ( NewName_String, "" ) != 0 )
					{
						success = Relabel( OldName_String, NewName_String );
						if ( !success )
							ErrorNum = IoErr();
						else
						{
							if ( cfg_PathExpand )
							{
								success = NameFromLock( lock, String, sizeof( String ) );
								if ( !success )
									ErrorNum = IoErr();
								else
									LoadDirectory( String, side );
							}
						}
					}
				}
			}

			UnLock( lock );
		}
		else
			ErrorNum = IoErr();
	}

	return( ErrorNum );
}

/*
**
** MakeAssign()
**
*/

int MakeAssign ( int side )
{
	BOOL Skip, Cancel;
	BOOL success;
	BPTR lock;
	char * Name_String, * cptr;
	char Device_String[32];
	int ErrorNum = 0;

	if ( global_DirLoaded[side] )
	{
		Name_String = StringRequester( GetCatStr( 96, "Enter assign name for directory" ), "", "", 31, 0, &Skip, &Cancel );
		if ( !Cancel && strlen( Name_String ) > 0 )
		{
			strcpy( Device_String, Name_String );
			cptr = strrchr( Device_String, ':' );
			if ( cptr != NULL )
				*cptr = '\0';
			if ( strlen( Device_String ) > 0 )
			{
				lock = Lock( GetPath( side ), ACCESS_READ );
				if ( lock )
				{
					success = AssignLock( Device_String, lock );
					if ( !success )
						ErrorNum = IoErr();
				}
				else
					ErrorNum = IoErr();
			}
		}
	}
	return( ErrorNum );
}

/*
**
** Select()
**
*/

void Select ( int side )
{
	ULONG i, Entries_ULONG;
	char * Pattern_String, * Pattern_Token;
	__aligned struct FileInfoBlock * fib;
	BOOL Skip, Cancel;

	if ( global_DirLoaded[side] )
	{
		Pattern_String = StringRequester( GetCatStr( 97, "Enter select pattern" ), global_Pattern_String, ":/", 81, 0, &Skip, &Cancel );
		if ( !Cancel && strlen( Pattern_String ) > 0 )
		{
			strcpy( global_Pattern_String, Pattern_String );
			Pattern_Token = malloc( 256 );
			if ( Pattern_Token )
			{
				ParsePatternNoCase( Pattern_String, Pattern_Token, 256 );
				get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );
				for ( i = 0; i < Entries_ULONG; i++ )
				{
					DoMethod( lv_Directory[side], MUIM_List_GetEntry, i, &fib );
					if ( MatchPatternNoCase( Pattern_Token, fib -> fib_FileName ) )
						DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_On, NULL );
				}
				free( Pattern_Token );
			}
		}
	}
}

/*
**
** Update()
**
*/

void Update ( int side )
{
	ULONG i,j, Entries_ULONG, EntriesSrc_ULONG;
	__aligned struct FileInfoBlock * srcfib;
	__aligned struct FileInfoBlock * fib;

	if ( global_DirLoaded[side] )
	{
		get( lv_Directory[side], MUIA_List_Entries, &EntriesSrc_ULONG );
		for ( j = 0; j < EntriesSrc_ULONG; j++ )
		{
			DoMethod( lv_Directory[side], MUIM_List_GetEntry, j, &srcfib );
			{
				get( lv_Directory[OtherSide(side)], MUIA_List_Entries, &Entries_ULONG );
				for ( i = 0; i < Entries_ULONG; i++ )
				{
					DoMethod( lv_Directory[OtherSide(side)], MUIM_List_GetEntry, i, &fib );
					if ( stricmp( srcfib -> fib_FileName , fib -> fib_FileName ) == 0 )
						DoMethod( lv_Directory[side], MUIM_List_Select, j, MUIV_List_Select_On, NULL );
				}
			}
		}
	}
}

/*
**
** Icon()
**
*/

void Icon ( int side )
{
	ULONG i,j, Entries_ULONG, Selection_State;
	char Info_String[41];
	__aligned struct FileInfoBlock * fib[2];

	if ( global_DirLoaded[side] )
	{
		get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );
		for ( i = 0; i < Entries_ULONG; i++ )
		{
			DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
			if ( Selection_State == MUIV_List_Select_On )
			{
				DoMethod( lv_Directory[side], MUIM_List_GetEntry, i, &fib[0] );
				sprintf( Info_String, "%s.info", fib[0] -> fib_FileName );
				for ( j = 0; j < Entries_ULONG; j++ )
				{
					DoMethod( lv_Directory[side], MUIM_List_GetEntry, j, &fib[1] );
					if ( stricmp( Info_String, fib[1] -> fib_FileName ) == 0 ) DoMethod( lv_Directory[side], MUIM_List_Select, j, MUIV_List_Select_On, NULL );
				}
			}
		}
	}
}

/*
**
** Change()
**
*/

void Change ( int side )
{
	BOOL Running = TRUE, NoChange = FALSE;
	APTR wi_Change, bt_Okay, bt_Cancel, cy_Sort, cy_First, cy_HighLow;
	ULONG signal, num;
	const char *cya_Sort[] = { "X", "X", "X", NULL };
	const char *cya_First[] = { "X", "X", "X", NULL };
	const char *cya_HighLow[] = { "X", "X", NULL };

	cya_Sort[0] = GetCatStr( 98, "Name" );
	cya_Sort[1] = GetCatStr( 99, "Date" );
	cya_Sort[2] = GetCatStr( 100, "Size" );
	cya_First[0] = GetCatStr( 101, "Dirs" );
	cya_First[1] = GetCatStr( 102, "Files" );
	cya_First[2] = GetCatStr( 103, "Mixed" );
	cya_HighLow[0] = GetCatStr( 128, "High" );
	cya_HighLow[1] = GetCatStr( 129, "Low" );

	wi_Change = WindowObject,
		MUIA_Window_ID, 4,
		MUIA_Window_Title, GetCatStr( 104, "Change" ),
		MUIA_Window_Menu, MUIV_Window_Menu_NoMenu,
		MUIA_Window_SizeGadget, FALSE,
		WindowContents, VGroup,
			Child, HGroup, MUIA_Group_SameSize, TRUE,
				Child, cy_Sort = Radio( GetCatStr( 105, "Sort" ), cya_Sort ),
				Child, cy_First = Radio( GetCatStr( 106, "First" ), cya_First ),
				Child, cy_HighLow = Radio( GetCatStr( 130, "Order" ), cya_HighLow ),
				End,
			Child, HGroup,
				MUIA_Group_SameSize, TRUE,
				Child, bt_Okay = SimpleButton(GetCatStr( 72, "_Okay" ) ),
				Child, HSpace(0),
				Child, bt_Cancel = SimpleButton(GetCatStr( 73, "_Cancel" ) ),
				End,
			End,
		End;

	if ( !wi_Change )
		Fail();

	DoMethod( wi_Change, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2);
	DoMethod( bt_Okay,   MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 1);
	DoMethod( bt_Cancel, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2);
	DoMethod( app_RumorOpus, OM_ADDMEMBER, wi_Change );
	set(cy_Sort, MUIA_Radio_Active, cfg_SortType[side] );
	set(cy_First, MUIA_Radio_Active, cfg_FirstType[side] );

	if ( !cfg_SortHighLow[side] )
		set(cy_HighLow, MUIA_Radio_Active, 0 );
	else
		set(cy_HighLow, MUIA_Radio_Active, 1 );

	set( wi_Change, MUIA_Window_Open, TRUE );
	while ( Running )
	{
		switch ( DoMethod( app_RumorOpus, MUIM_Application_Input, &signal ) )
		{
			case MUIV_Application_ReturnID_Quit:
				global_QuitProgram = QuitRequester();
				if ( global_QuitProgram )
					Running = FALSE;
				break;

			case 1:
				Running = FALSE;
				break;

			case 2:
				NoChange = TRUE;
				Running = FALSE;
				break;
		}
		if (signal) Wait(signal);
	}
	set(wi_Change, MUIA_Window_Open, FALSE);

	if ( !NoChange )
	{
		get( cy_Sort, MUIA_Radio_Active, &cfg_SortType[side] );
		get( cy_First, MUIA_Radio_Active, &cfg_FirstType[side] );
		get( cy_HighLow, MUIA_Radio_Active, &num );
		if ( num == 0 )
			cfg_SortHighLow[side] = FALSE;
		else
			cfg_SortHighLow[side] = TRUE;

		set( lv_Directory[side], MUIA_Dirlist_SortType, cfg_SortType[side] );
		set( lv_Directory[side], MUIA_Dirlist_SortDirs, cfg_FirstType[side] );
		set( lv_Directory[side], MUIA_Dirlist_SortHighLow, cfg_SortHighLow[side] );
	}

	DoMethod(app_RumorOpus, OM_REMMEMBER, wi_Change );
	MUI_DisposeObject( wi_Change );
}

/*
**
** DiskInfo()
**
*/

void DiskInfo ( int side )
{
	BOOL Running=TRUE;
	char * cptr;
	char Device_String[32], String[1024], Status_String[21], Bytes_String[8][21];
	ULONG BytesTotal_ULONG, BytesUsed_ULONG, Density_ULONG, signal;
	BPTR lock;
	APTR wi_Disk, tx_TextLeft, tx_TextRight, bt_Okay, bt_Cancel;
	struct InfoData *pid;

	if ( global_DirLoaded[side] )
	{
		lock = Lock( GetPath(side), ACCESS_READ );
		if ( lock )
		{
			pid = malloc( sizeof( struct InfoData ) );
			if ( pid )
			{
				Info( lock, pid );

				strcpy( String, GetPath( side ) );
				cptr = strrchr( String, ':' );
				cptr++;
				* cptr = '\0';
				strcpy(	Device_String, String );

				BytesTotal_ULONG = ( pid -> id_NumBlocks ) * ( pid -> id_BytesPerBlock );
				BytesUsed_ULONG = ( pid -> id_NumBlocksUsed ) * ( pid -> id_BytesPerBlock );
				Density_ULONG = ( pid -> id_BytesPerBlock );
				switch ( pid -> id_DiskState )
				{
					case ID_WRITE_PROTECTED:
						strcpy( Status_String, GetCatStr( 107, "Read Only" ) );
						break;

					case ID_VALIDATING:
						strcpy( Status_String, GetCatStr( 108, "Validating" ) );
						break;

					case ID_VALIDATED:
						strcpy( Status_String, GetCatStr( 109, "Read/Write" ) );
						break;
				}
				strcpy( Bytes_String[0], NumberToString( ( BytesTotal_ULONG / 1024 ) ) );
				strcpy( Bytes_String[1], NumberToString( BytesTotal_ULONG / Density_ULONG ) );
				strcpy( Bytes_String[2], NumberToString( BytesTotal_ULONG ) );
				strcpy( Bytes_String[3], NumberToString( BytesUsed_ULONG / Density_ULONG ) );
				strcpy( Bytes_String[4], NumberToString( BytesUsed_ULONG ) );
				strcpy( Bytes_String[5], NumberToString( ( BytesTotal_ULONG - BytesUsed_ULONG ) / Density_ULONG ) );
				strcpy( Bytes_String[6], NumberToString( ( BytesTotal_ULONG - BytesUsed_ULONG ) ) );
				strcpy( Bytes_String[7], NumberToString( Density_ULONG ) );
				sprintf( String, GetCatStr( 110, "%s\n%sK\n%s blocks or %s bytes\n%s blocks or %s bytes\n%s blocks or %s bytes\n%ld%%\n%s bytes/block\n%s" ), Device_String, Bytes_String[0] , Bytes_String[1], Bytes_String[2], Bytes_String[3], Bytes_String[4], Bytes_String[5], Bytes_String[6], ( BytesUsed_ULONG / ( BytesTotal_ULONG / 100 ) ), Bytes_String[7], Status_String );

				wi_Disk = WindowObject,
					MUIA_Window_ID, 5,
					MUIA_Window_Title, GetCatStr( 111, "Device Information" ),
					MUIA_Window_Menu, MUIV_Window_Menu_NoMenu,
					WindowContents, VGroup,
						Child, HGroup, MUIA_Group_SameSize, FALSE,
							TextFrame,
							MUIA_Background, MUII_TextBack,
							Child, tx_TextLeft = TextObject,
								TextFrame,
								MUIA_FramePhantomHoriz, TRUE,
								MUIA_Background, MUII_TextBack,
								MUIA_Text_Contents, GetCatStr( 112, "Name:\nSize:\nTotal:\nUsed:\nFree:\nFull:\nDensity:\nStatus:" ),
								MUIA_Text_SetMax, TRUE,
								End,
							Child, tx_TextRight = TextObject,
								TextFrame,
								MUIA_FramePhantomHoriz, TRUE,
								MUIA_Background, MUII_TextBack,
								MUIA_Text_Contents, String,
								End,
							End,
						Child, HGroup,
							MUIA_Group_SameSize, TRUE,
							Child, bt_Okay = SimpleButton(GetCatStr( 72, "_Okay" ) ),
							Child, HSpace(0),
							Child, HSpace(0),
							Child, bt_Cancel = SimpleButton(GetCatStr( 73, "_Cancel" ) ),
							End,
						End,
					End;

				if( !wi_Disk )
					Fail();

				DoMethod( wi_Disk, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, 1 );
				DoMethod( bt_Okay, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 1 );
				DoMethod( bt_Cancel, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 1 );
				DoMethod( app_RumorOpus, OM_ADDMEMBER, wi_Disk );
				set( wi_Disk, MUIA_Window_Open, TRUE );
				set( wi_Disk, MUIA_Window_ActiveObject, bt_Okay );

				while ( Running )
				{
					switch ( DoMethod( app_RumorOpus, MUIM_Application_Input, &signal ) )
					{
						case MUIV_Application_ReturnID_Quit:
							global_QuitProgram = QuitRequester();
							if( global_QuitProgram )
								Running = FALSE;
							break;

						case 1:
							Running = FALSE;
							break;
					}
					if (signal) Wait(signal);
				}

				set( wi_Disk, MUIA_Window_Open, FALSE );
				DoMethod( app_RumorOpus, OM_REMMEMBER, wi_Disk );
				MUI_DisposeObject( wi_Disk );
				free( pid );
			}
			UnLock(lock);
		}
	}
}

/*
**
** Bytes()
**
*/

char * Bytes ( int side )
{
	ULONG i, k = 0, Entries_ULONG, Active_LONG, Selection_State, Iconified;
	char FileName_String[512], String[512], Return_String[256], Size_String[3][21];
	BOOL Action_BOOL = FALSE;
	__aligned struct FileInfoBlock * fib;
	ULONG Total_ULONG, Files_ULONG = 0, Dirs_ULONG = 0, Size_ULONG = 0;
	int ErrorNum = 0;

	if ( global_DirLoaded[side] )
	{
		get( lv_Directory[side], MUIA_List_Active, &Active_LONG );
		get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );
		for ( i = 0; i < Entries_ULONG; i++ )
		{
			DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
			if ( Selection_State == MUIV_List_Select_On ) k++;
		}
		if ( k == 0 && Active_LONG != MUIV_List_Active_Off)
		{
			k = 1;
			DoMethod( lv_Directory[side], MUIM_List_Select, Active_LONG, MUIV_List_Select_On, NULL );
		}
		set( ga_Gauge, MUIA_Gauge_Current, 0 );
		set( ga_Gauge, MUIA_Gauge_Max, k );
		if ( k > 0 )
			set( wi_Progress, MUIA_Window_Open, TRUE );
		k = 0;

		for ( i = 0; i < Entries_ULONG; i++ )
		{
			DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
			if ( Selection_State == MUIV_List_Select_On )
			{
				DoMethod( lv_Directory[side], MUIM_List_GetEntry, i, &fib );
				sprintf( FileName_String, "%s%s", GetPath( side ), fib -> fib_FileName );

				sprintf( String, GetCatStr( 113, "Sizing '%s'..." ), FileName_String );
				set( bt_StatusBar, MUIA_Text_Contents, String );

				Total_ULONG = 0;
				if ( fib -> fib_DirEntryType < 0 )
				{
					Total_ULONG = Total_ULONG + fib -> fib_Size;
					Files_ULONG++;
				}
				if (fib->fib_DirEntryType > 0)
				{
					ErrorNum = BytesDirectory( FileName_String, &Total_ULONG, &Files_ULONG, &Dirs_ULONG );
					Dirs_ULONG++;
				}
				Size_ULONG = Size_ULONG + Total_ULONG;

				k++;
				set( ga_Gauge, MUIA_Gauge_Current, k );
				Action_BOOL = TRUE;
			}

			if ( ErrorNum != 0 )
				break;
		}
		set( wi_Progress, MUIA_Window_Open, FALSE );
		if ( Action_BOOL )
		{
			if ( ErrorNum == 0 )
			{
				strcpy( Size_String[0], NumberToString( Size_ULONG ) );
				strcpy( Size_String[1], NumberToString( Files_ULONG ) );
				strcpy( Size_String[2], NumberToString( Dirs_ULONG ) );
				sprintf( Return_String, GetCatStr( 114, "%s Bytes in %s Files and %s Directories" ), Size_String[0], Size_String[1], Size_String[2] );
				get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
				if ( Iconified )
				{
					switch ( cfg_Completed )
					{
						case 1:
							DisplayBeep( 0 );
							break;

						case 2:
							set( app_RumorOpus, MUIA_Application_Iconified, 0 );
							break;
					}
				}
			}
			else
				strcpy( Return_String, Error( ErrorNum ) );

			return( Return_String );
		}
	}
	return( NULL );
}

/*
**
** Fit()
**
*/

char * Fit ( int side, BOOL window )
{
	ULONG i, k = 0, Entries_ULONG, Active_LONG, Selection_State, Iconified;
	char FileName_String[512], String[512], Return_String[256], Size_String[3][21];
	BOOL Action_BOOL = FALSE;
	__aligned struct FileInfoBlock * fib;
	struct InfoData * pid;
	ULONG Total_ULONG, Files_ULONG = 0, Dirs_ULONG = 0, Size_ULONG = 0, BlockSize_ULONG;
	ULONG BytesTotal_ULONG, BytesUsed_ULONG, FreeBytes_ULONG;
	BPTR lock;
	int ErrorNum = 0;

	if ( global_DirLoaded[Left_Side] && global_DirLoaded[Right_Side] )
	{
		lock = Lock( GetPath( OtherSide( Active_Side ) ), ACCESS_READ );
		if ( lock )
		{
			pid = malloc( sizeof( struct InfoData ) );
			if ( pid )
			{
				Info( lock, pid );
				BlockSize_ULONG = pid -> id_BytesPerBlock;

				get( lv_Directory[side], MUIA_List_Active, &Active_LONG );
				get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );
				if ( window )
				{
					for ( i = 0; i < Entries_ULONG; i++ )
					{
						DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
						if ( Selection_State == MUIV_List_Select_On ) k++;
					}
					if ( k == 0 && Active_LONG != MUIV_List_Active_Off)
					{
						k = 1;
						DoMethod( lv_Directory[side], MUIM_List_Select, Active_LONG, MUIV_List_Select_On, NULL );
					}
					set( ga_Gauge, MUIA_Gauge_Current, 0 );
					set( ga_Gauge, MUIA_Gauge_Max, k );
					if ( k > 0 )
						set( wi_Progress, MUIA_Window_Open, TRUE );
					k = 0;
				}

				for ( i = 0; i < Entries_ULONG; i++ )
				{
					DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
					if ( Selection_State == MUIV_List_Select_On )
					{
						DoMethod( lv_Directory[side], MUIM_List_GetEntry, i, &fib );
						sprintf( FileName_String, "%s%s", GetPath( side ), fib -> fib_FileName );

						sprintf( String, GetCatStr( 113, "Sizing '%s'..." ), FileName_String );
						set( bt_StatusBar, MUIA_Text_Contents, String );

						Total_ULONG = 0;
						if ( fib -> fib_DirEntryType < 0 )
						{
							if ( ( fib -> fib_Size % BlockSize_ULONG ) > 0 )
								Total_ULONG = Total_ULONG + ( ( fib -> fib_Size / BlockSize_ULONG ) + 1 );
							else
								Total_ULONG = Total_ULONG + ( fib -> fib_Size / BlockSize_ULONG );
							Files_ULONG++;
						}
						if (fib->fib_DirEntryType > 0)
						{
							ErrorNum = FitDirectory( FileName_String, &Total_ULONG, &Files_ULONG, &Dirs_ULONG, BlockSize_ULONG );
							Dirs_ULONG++;
						}
						Size_ULONG = Size_ULONG + Total_ULONG;

						if ( window )
						{
							k++;
							set( ga_Gauge, MUIA_Gauge_Current, k );
						}
						Action_BOOL = TRUE;
					}

					if ( ErrorNum != 0 )
						break;
				}
				if ( window )
					set( wi_Progress, MUIA_Window_Open, FALSE );
				if ( Action_BOOL )
				{
					if ( ErrorNum == 0 )
					{
						BytesTotal_ULONG = ( pid -> id_NumBlocks ) * ( pid -> id_BytesPerBlock );
						BytesUsed_ULONG = ( pid -> id_NumBlocksUsed ) * ( pid -> id_BytesPerBlock );
						FreeBytes_ULONG = BytesTotal_ULONG - BytesUsed_ULONG;
						Size_ULONG = ( Size_ULONG + Files_ULONG + Dirs_ULONG ) * BlockSize_ULONG;
						strcpy( Size_String[0], NumberToString( Size_ULONG ) );
						strcpy( Size_String[1], NumberToString( FreeBytes_ULONG ) );
						if ( Size_ULONG > FreeBytes_ULONG )
						{
							strcpy( Size_String[2], NumberToString( Size_ULONG - FreeBytes_ULONG ) );
							sprintf( Return_String, GetCatStr( 115, "NO!  Bytes: %s required, %s available, %s short" ), Size_String[0], Size_String[1], Size_String[2] );
						}
						else
						{
							strcpy( Size_String[2], NumberToString( FreeBytes_ULONG - Size_ULONG ) );
							sprintf( Return_String, GetCatStr( 116, "YES!  Bytes: %s required, %s available, %s to spare" ), Size_String[0], Size_String[1], Size_String[2] );
						}
					}
					else
						strcpy( Return_String, Error( ErrorNum ) );
				}
				if ( window )
				{
					get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
					if ( Iconified )
						switch ( cfg_Completed )
						{
							case 1:
								DisplayBeep( 0 );
								break;

							case 2:
								set( app_RumorOpus, MUIA_Application_Iconified, 0 );
								break;
						}
				}
				free( pid );
			}
			UnLock( lock );
		}
		if( Action_BOOL )
			return( Return_String );
	}
	return( NULL );
}

/*
**
** Expand()
**
*/

void Expand ( int side )
{
	char Path_String[512], Expanded_String[512];
	char * cptr;

	if ( global_DirLoaded[side] )
	{
		strcpy( Path_String, GetPath( side ) );
		cptr = strrchr( Path_String, ':' );
		*cptr = '/';
		sprintf( Expanded_String, "ARC:%s", Path_String );

		LoadDirectory( Expanded_String, side );
	}
}

/*
**
** Shrink()
**
*/

void Shrink ( int side )
{
	char Path_String[512], Shrinked_String[512], Device_String[512], Archives_String[32];
	char * cptr;
	BPTR lock;

	if ( global_DirLoaded[side] )
	{
		lock = Lock( "ARC:", ACCESS_READ );
		if ( lock )
		{
			if ( cfg_PathExpand )
				NameFromLock ( lock, Archives_String, sizeof( Archives_String ) );
			else
				strcpy( Archives_String, "ARC:" );

			strcpy( Path_String, GetPath( side ) );
			if ( strstr( Path_String, Archives_String ) != NULL )
			{
				strcpy( Device_String, Path_String );
				cptr = strrchr( Device_String, ':' );
				cptr++;
				*cptr = '\0';

				strmid( Path_String, Shrinked_String, ( strlen( Device_String ) + 1 ), ( strlen( Path_String ) ) - ( strlen( Device_String ) ) );
				cptr = strchr( Shrinked_String, '/' );
				*cptr = ':';

				LoadDirectory( Shrinked_String, side );
			}
			UnLock( lock );
		}
	}
}
