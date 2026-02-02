#include "Includes.h"
#include "DoFunction.h"

/*
** Context structure for passing state between handler functions
*/
typedef struct {
	int side;
	char * Argument;
	char FileName_String[512];
	char Target_String[512];
	char String[1024];
	__aligned struct FileInfoBlock * fib;
	int ErrorNum;
	BOOL Skip;
	BOOL Cancel;
	BOOL Action_BOOL;
	BOOL ReloadSrc_BOOL;
	BOOL ReloadDst_BOOL;
	BOOL NoBreak_BOOL;
	BOOL NoDeselect_BOOL;
} FuncContext;

/*
** Handler: command_entries
*/
static void HandleCommand( FuncContext * ctx, BOOL Request_Dst )
{
	int rc;

	if ( Request_Dst )
	{
		sprintf( Status_String, GetCatStr( 46, "Executing Command for '%s'..." ), ctx->FileName_String );
		set( bt_StatusBar, MUIA_Text_Contents, Status_String );
		rc = ExecuteCommand( ctx->Argument, ctx->FileName_String );
		if ( rc != 0 && !ctx->NoBreak_BOOL )
			ctx->ErrorNum = ERR_CMD_FAILED;
		ctx->Action_BOOL = TRUE;
	}
	else
		ctx->Cancel = TRUE;
}

/*
** Handler: action_entries
*/
static void HandleAction( FuncContext * ctx )
{
	if ( ctx->fib->fib_DirEntryType < 0 )
	{
		sprintf( Status_String, GetCatStr( 47, "Performing FileAction for '%s'..." ), ctx->FileName_String );
		set( bt_StatusBar, MUIA_Text_Contents, Status_String );
		ctx->ErrorNum = FileAction( ctx->FileName_String );
		if ( ctx->ErrorNum == ERR_CMD_FAILED && ctx->NoBreak_BOOL )
			ctx->ErrorNum = 0;
	}
}

/*
** Handler: copydev_entries
*/
static void HandleCopyDev( FuncContext * ctx )
{
	char * NewName_String;

	sprintf( Status_String, GetCatStr( 48, "Copying '%s'..." ), ctx->FileName_String );
	set( bt_StatusBar, MUIA_Text_Contents, Status_String );

	if ( !ctx->Action_BOOL )
	{
		NewName_String = StringRequester( GetCatStr( 49, "Copy selected files to device" ), "PRT:", "/", 31, 0, &ctx->Skip, &ctx->Cancel );
		if ( strlen( NewName_String ) == 0 )
			ctx->Cancel = TRUE;
		else
			strcpy( ctx->Target_String, NewName_String );
	}

	if ( !ctx->Skip && !ctx->Cancel )
		ctx->ErrorNum = Copy( ctx->FileName_String, ctx->Target_String );

	if ( ctx->ErrorNum == ERR_SKIPPED )
		ctx->ErrorNum = 0;
}

/*
** Handler: delete_entries
*/
static void HandleDelete( FuncContext * ctx )
{
	int rc;

	if ( cfg_DelVerify && !ctx->Action_BOOL )
	{
		rc = MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 50, "Request" ), GetCatStr( 51, "*_Delete|_Cancel" ), GetCatStr( 52, "Are you sure you want\nto delete these entries?" ), TAG_DONE );
		if ( rc == 0 )
			ctx->Cancel = TRUE;
	}

	if ( !ctx->Cancel )
	{
		sprintf( Status_String, GetCatStr( 53, "Deleting '%s'..." ), ctx->FileName_String );
		set( bt_StatusBar, MUIA_Text_Contents, Status_String );
		ctx->ErrorNum = Delete( ctx->FileName_String );
		if ( ctx->ErrorNum == 0 )
			ctx->ReloadSrc_BOOL = TRUE;
	}
}

/*
** Handler: unarc_entries
*/
static void HandleUnarc( FuncContext * ctx )
{
	int TypeNum, rc;

	if ( ctx->fib->fib_DirEntryType < 0 && global_DirLoaded[OtherSide(ctx->side)] )
	{
		sprintf( Status_String, GetCatStr( 77, "Extracting '%s' to '%s'..." ), ctx->FileName_String, GetPath( OtherSide( ctx->side ) ) );
		set( bt_StatusBar, MUIA_Text_Contents, Status_String );
		TypeNum = RecogArchive( ctx->FileName_String );
		if ( TypeNum != -1 )
			strcpy( ctx->String, cfg_RecogCommandB[TypeNum] );
	}

	if ( strlen( ctx->String ) > 0 )
	{
		BOOL Request_Dst;
		ParseMisc( ctx->Argument, &ctx->NoBreak_BOOL, &ctx->NoDeselect_BOOL );
		ParseReload( ctx->String, &ctx->ReloadSrc_BOOL, &ctx->ReloadDst_BOOL );
		ParseNeedTarget( ctx->side, ctx->String, &Request_Dst );

		if ( Request_Dst )
		{
			rc = ExecuteCommand( ctx->String, ctx->FileName_String );
			if ( rc != 0 && !ctx->NoBreak_BOOL )
				ctx->ErrorNum = ERR_CMD_FAILED;
			ctx->Action_BOOL = TRUE;
		}
		else
			ctx->Cancel = TRUE;
	}
}

/*
** Handler: copy/move operations
*/
static void HandleCopyMove( FuncContext * ctx, int Operation )
{
	char SrcDevice_String[512], DstDevice_String[512];
	char Help_String_A[512], Help_String_B[512];
	char * cptr, * NewName_String;
	BPTR lock;
	BOOL success, Fitting_BOOL = TRUE, Move_BOOL = FALSE, As_BOOL = FALSE, SameDevice_BOOL = FALSE;
	LONG Opened, Iconified, Proceed;

	if ( !global_DirLoaded[OtherSide(ctx->side)] )
	{
		ctx->Cancel = TRUE;
		return;
	}

	if ( !ctx->Action_BOOL )
	{
		ctx->ReloadDst_BOOL = TRUE;

		strcpy( SrcDevice_String, GetPath( ctx->side ) );
		cptr = strrchr( SrcDevice_String, ':' );
		if ( cptr ) { cptr++; *cptr = '\0'; }

		strcpy( DstDevice_String, GetPath( OtherSide( ctx->side ) ) );
		cptr = strrchr( DstDevice_String, ':' );
		if ( cptr ) { cptr++; *cptr = '\0'; }

		if ( stricmp( DstDevice_String, SrcDevice_String ) == 0 )
			SameDevice_BOOL = TRUE;

		if ( Operation == move_entries || Operation == moveas_entries )
			Move_BOOL = TRUE;

		if ( cfg_CheckFit && !( SameDevice_BOOL && Move_BOOL ) )
		{
			if ( cfg_PathExpand )
			{
				lock = Lock( "RAM:", ACCESS_READ );
				if ( lock )
				{
					success = NameFromLock( lock, ctx->String, sizeof( ctx->String ) );
					if ( !success )
						ctx->ErrorNum = IoErr();
					UnLock( lock );
				}
				else
					ctx->ErrorNum = IoErr();
			}
			else
				strcpy( ctx->String, "RAM:" );

			if ( ctx->ErrorNum == 0 )
			{
				if ( strstr( GetPath( OtherSide( ctx->side ) ), ctx->String ) == NULL )
					if ( strstr( Fit( ctx->side, FALSE ), GetCatStr( 54, "NO!" ) ) != NULL )
						Fitting_BOOL = FALSE;
			}
		}

		if ( Operation == copyas_entries || Operation == moveas_entries )
			As_BOOL = TRUE;
	}

	get( wi_Progress, MUIA_Window_Open, &Opened );
	get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );

	if ( ctx->ErrorNum != 0 || ( !Opened && !Iconified ) )
	{
		ctx->Cancel = TRUE;
		return;
	}

	if ( !Fitting_BOOL && !( SameDevice_BOOL && Move_BOOL ) )
	{
		ctx->ErrorNum = ERR_NO_DISK_SPACE;
		return;
	}

	if ( !As_BOOL )
		sprintf( Status_String, GetCatStr( 55, "Copying '%s' to '%s%s'..." ), ctx->FileName_String, GetPath( OtherSide( ctx->side ) ), FilePart( ctx->FileName_String ) );
	else
		sprintf( Status_String, GetCatStr( 56, "Copying '%s'..." ), ctx->FileName_String );
	set( bt_StatusBar, MUIA_Text_Contents, Status_String );

	if ( As_BOOL )
	{
		if ( Operation == copyas_entries )
			sprintf( ctx->String, GetCatStr( 57, "Copy '%s' as" ), FilePart( ctx->FileName_String ) );
		else
			sprintf( ctx->String, GetCatStr( 58, "Move '%s' as" ), FilePart( ctx->FileName_String ) );
		NewName_String = StringRequester( ctx->String, FilePart( ctx->FileName_String ), ":/", 31, 1, &ctx->Skip, &ctx->Cancel );
		strcpy( ctx->String, NewName_String );
	}
	else
		strcpy( ctx->String, FilePart( ctx->FileName_String ) );

	if ( ctx->Skip || ctx->Cancel || strlen( ctx->String ) == 0 )
		return;

	sprintf( ctx->Target_String, "%s%s", GetPath( OtherSide( ctx->side ) ), ctx->String );

	if ( Move_BOOL && SameDevice_BOOL )
	{
		sprintf( Help_String_A, "/%s/", ctx->FileName_String );
		sprintf( Help_String_B, "/%s/", ctx->Target_String );

		if ( strstr( Help_String_B, Help_String_A ) != NULL || stricmp( ctx->FileName_String, ctx->Target_String ) == 0 )
		{
			ctx->ErrorNum = ERR_CANT_DELETE_SRC;
			return;
		}

		if ( Exists( ctx->Target_String ) )
		{
			Proceed = Overwrite( ctx->FileName_String, ctx->Target_String );
			if ( Proceed == 1 )
				ctx->ErrorNum = Delete( ctx->Target_String );
			else
			{
				ctx->ErrorNum = ERR_SKIPPED;
				if ( cfg_Overwrite == 1 )
					ctx->ErrorNum = ERR_FILE_EXISTS;
			}
		}
		else
			Proceed = 1;

		if ( Proceed == 1 && ctx->ErrorNum == 0 )
		{
			success = Rename( ctx->FileName_String, ctx->Target_String );
			if ( !success )
				ctx->ErrorNum = IoErr();
			else
				ctx->ReloadSrc_BOOL = TRUE;
		}
	}
	else
	{
		ctx->ErrorNum = Copy( ctx->FileName_String, ctx->Target_String );
		if ( ctx->ErrorNum == 0 )
		{
			ctx->Action_BOOL = TRUE;
			if ( Move_BOOL )
			{
				if ( Exists( ctx->Target_String ) )
					ctx->ErrorNum = Delete( ctx->FileName_String );
				if ( ctx->ErrorNum == 0 )
					ctx->ReloadSrc_BOOL = TRUE;
			}
		}
		if ( ctx->ErrorNum == ERR_SKIPPED )
			ctx->ErrorNum = 0;
	}
}

/*
** Handler: rename_entries
*/
static void HandleRename( FuncContext * ctx )
{
	char * NewName_String;
	BOOL success, Done = TRUE;

	sprintf( Status_String, GetCatStr( 59, "Renaming '%s'..." ), ctx->FileName_String );
	set( bt_StatusBar, MUIA_Text_Contents, Status_String );
	sprintf( ctx->String, GetCatStr( 60, "Rename '%s' as" ), ctx->fib->fib_FileName );
	NewName_String = StringRequester( ctx->String, ctx->fib->fib_FileName, ":/", 31, 1, &ctx->Skip, &ctx->Cancel );

	if ( ctx->Skip || ctx->Cancel || strlen( NewName_String ) == 0 )
		return;

	sprintf( ctx->Target_String, "%s%s", GetPath( ctx->side ), NewName_String );

	if ( stricmp( ctx->FileName_String, ctx->Target_String ) != 0 )
	{
		if ( Exists( ctx->Target_String ) )
		{
			if ( Overwrite( ctx->FileName_String, ctx->Target_String ) == 1 )
				ctx->ErrorNum = Delete( ctx->Target_String );
			else
				Done = FALSE;
		}
	}

	if ( ctx->ErrorNum == 0 && Done )
	{
		success = Rename( ctx->FileName_String, ctx->Target_String );
		if ( !success )
			ctx->ErrorNum = IoErr();
		else
			ctx->ReloadSrc_BOOL = TRUE;
	}
}

/*
** Handler: dup_entries
*/
static void HandleDuplicate( FuncContext * ctx )
{
	char * NewName_String;
	BOOL Done = TRUE;

	sprintf( Status_String, GetCatStr( 61, "Duplicating '%s'..." ), ctx->FileName_String );
	set( bt_StatusBar, MUIA_Text_Contents, Status_String );
	sprintf( ctx->String, GetCatStr( 62, "Duplicate '%s' as" ), ctx->fib->fib_FileName );
	NewName_String = StringRequester( ctx->String, ctx->fib->fib_FileName, ":/", 31, 1, &ctx->Skip, &ctx->Cancel );

	if ( ctx->Skip || ctx->Cancel || strlen( NewName_String ) == 0 )
		return;

	sprintf( ctx->Target_String, "%s%s", GetPath( ctx->side ), NewName_String );

	if ( stricmp( ctx->FileName_String, ctx->Target_String ) != 0 )
	{
		if ( Exists( ctx->Target_String ) )
		{
			if ( Overwrite( ctx->FileName_String, ctx->Target_String ) == 1 )
				ctx->ErrorNum = Delete( ctx->Target_String );
			else
				Done = FALSE;
		}
	}

	if ( ctx->ErrorNum == 0 && Done )
	{
		ctx->ErrorNum = Copy( ctx->FileName_String, ctx->Target_String );
		if ( ctx->ErrorNum == 0 )
			ctx->ReloadSrc_BOOL = TRUE;
		if ( ctx->ErrorNum == ERR_SKIPPED )
			ctx->ErrorNum = 0;
	}
}

/*
** Handler: touch_entries
*/
static void HandleTouch( FuncContext * ctx )
{
	struct DateStamp * date;
	BOOL success;

	date = malloc( sizeof( struct DateStamp ) );
	if ( date )
	{
		DateStamp( date );
		sprintf( Status_String, GetCatStr( 63, "Touching '%s'..." ), ctx->FileName_String );
		set( bt_StatusBar, MUIA_Text_Contents, Status_String );
		success = SetFileDate( ctx->FileName_String, date );
		if ( !success )
			ctx->ErrorNum = IoErr();
		else
			ctx->ReloadSrc_BOOL = TRUE;
		free( date );
	}
}

/*
** Handler: comment_entries
*/
static void HandleComment( FuncContext * ctx, char * Version_String, BOOL * CommentAll_BOOL )
{
	char * NewName_String;
	BOOL success;

	sprintf( Status_String, GetCatStr( 64, "Setting comment for '%s'..." ), ctx->FileName_String );
	set( bt_StatusBar, MUIA_Text_Contents, Status_String );

	if ( !*CommentAll_BOOL )
	{
		sprintf( ctx->String, GetCatStr( 65, "Enter comment for '%s'" ), ctx->fib->fib_FileName );
		NewName_String = StringRequester( ctx->String, ctx->fib->fib_Comment, "", 80, 2, &ctx->Skip, &ctx->Cancel );
		strcpy( Version_String, NewName_String );
	}

	if ( ctx->Skip )
		*CommentAll_BOOL = TRUE;

	if ( !ctx->Cancel || *CommentAll_BOOL )
	{
		success = SetComment( ctx->FileName_String, Version_String );
		if ( !success )
			ctx->ErrorNum = IoErr();
		else
			ctx->ReloadSrc_BOOL = TRUE;
	}
}

/*
** Handler: setdate_entries
*/
static void HandleSetDate( FuncContext * ctx )
{
	struct DateTime * datetime;
	char Date_String[16], Time_String[10];
	char * NewName_String;
	BOOL success;
	int rc;

	sprintf( Status_String, GetCatStr( 66, "Setting datestamp for '%s'..." ), ctx->FileName_String );
	set( bt_StatusBar, MUIA_Text_Contents, Status_String );

	datetime = malloc( sizeof( struct DateTime ) );
	if ( !datetime )
	{
		ctx->ErrorNum = ERR_NO_MEMORY;
		return;
	}

	datetime->dat_Stamp = ctx->fib->fib_Date;
	datetime->dat_Format = FORMAT_DOS;
	datetime->dat_Flags = NULL;
	datetime->dat_StrDay = NULL;
	datetime->dat_StrDate = Date_String;
	datetime->dat_StrTime = Time_String;

	if ( DateToStr( datetime ) )
		sprintf( ctx->String, "%s %s", Date_String, Time_String );
	else
		ctx->ErrorNum = ERR_DATE_TO_STR;

	free( datetime );

	if ( ctx->ErrorNum != 0 )
		return;

	sprintf( Status_String, GetCatStr( 67, "Enter datestamp for '%s'" ), ctx->fib->fib_FileName );
	NewName_String = StringRequester( Status_String, ctx->String, "", 80, 1, &ctx->Skip, &ctx->Cancel );

	if ( ctx->Skip || ctx->Cancel || strlen( NewName_String ) == 0 )
		return;

	rc = sscanf( NewName_String, "%s %s", Date_String, Time_String );
	if ( rc != 2 )
	{
		ctx->ErrorNum = ERR_STR_TO_DATE;
		return;
	}

	datetime = malloc( sizeof( struct DateTime ) );
	if ( !datetime )
	{
		ctx->ErrorNum = ERR_NO_MEMORY;
		return;
	}

	datetime->dat_Format = FORMAT_DOS;
	datetime->dat_Flags = NULL;
	datetime->dat_StrDay = NULL;
	datetime->dat_StrDate = Date_String;
	datetime->dat_StrTime = Time_String;

	if ( StrToDate( datetime ) )
	{
		success = SetFileDate( ctx->FileName_String, &datetime->dat_Stamp );
		if ( !success )
			ctx->ErrorNum = IoErr();
		else
			ctx->ReloadSrc_BOOL = TRUE;
	}
	else
		ctx->ErrorNum = ERR_STR_TO_DATE;

	free( datetime );
}

/*
** Handler: info_entries
*/
static void HandleInfo( FuncContext * ctx )
{
	APTR wi_Information, tx_TextLeft, tx_TextRight, bt_Okay, bt_Cancel;
	__aligned struct FileInfoBlock * fib;
	struct DateTime * datetime;
	BPTR lock;
	ULONG signal;
	BOOL success;
	char Date_String[16], Time_String[10], ProtBits[10], Type_String[51], Size_String[21], Version_String[81];
	int l, TypeNum;

	sprintf( Status_String, GetCatStr( 68, "Showing information for '%s'..." ), ctx->FileName_String );
	set( bt_StatusBar, MUIA_Text_Contents, Status_String );

	fib = malloc( sizeof( struct FileInfoBlock ) );
	if ( !fib )
	{
		ctx->ErrorNum = ERR_NO_MEMORY;
		return;
	}

	lock = Lock( ctx->FileName_String, ACCESS_READ );
	if ( !lock )
	{
		ctx->ErrorNum = IoErr();
		free( fib );
		return;
	}

	Examine( lock, fib );

	datetime = malloc( sizeof( struct DateTime ) );
	if ( !datetime )
	{
		ctx->ErrorNum = ERR_NO_MEMORY;
		UnLock( lock );
		free( fib );
		return;
	}

	datetime->dat_Stamp = fib->fib_Date;
	datetime->dat_Format = FORMAT_DOS;
	datetime->dat_Flags = NULL;
	datetime->dat_StrDay = NULL;
	datetime->dat_StrDate = Date_String;
	datetime->dat_StrTime = Time_String;

	if ( !DateToStr( datetime ) )
	{
		ctx->ErrorNum = ERR_DATE_TO_STR;
		free( datetime );
		UnLock( lock );
		free( fib );
		return;
	}

	for ( l = 0; l < 8; l++ )
		ProtBits[7-l] = ( fib->fib_Protection ^ 0x0f ) & ( 1 << l ) ? "dewrapsh"[l] : '-';
	ProtBits[8] = 0;

	if ( fib->fib_DirEntryType > 0 )
	{
		strcpy( Type_String, GetCatStr( 69, "Directory" ) );
		TypeNum = -1;
	}
	else
	{
		TypeNum = Recog( ctx->FileName_String );
		if ( TypeNum == -1 )
			TypeNum = RecogArchive( ctx->FileName_String );

		if ( TypeNum == -1 )
			strcpy( Type_String, "Binary File" );
		else if ( TypeNum == -2 )
			strcpy( Type_String, "ASCII File" );
		else if ( TypeNum == -3 )
			strcpy( Type_String, "Executable File" );
		else if ( TypeNum == -4 )
			strcpy( Type_String, "XPK/PP Crunched File" );
		else
			strcpy( Type_String, cfg_RecogName[TypeNum] );
	}

	strcpy( Size_String, NumberToString( fib->fib_Size ) );

	if ( TypeNum == -3 || TypeNum == -2 )
		strcpy( Version_String, GetVersion( ctx->FileName_String ) );
	else
		strcpy( Version_String, GetCatStr( 126, "unknown" ) );

	sprintf( ctx->String, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s",
		ctx->FileName_String, Date_String, Time_String, Size_String,
		ProtBits, Version_String, Type_String, fib->fib_Comment );

	free( datetime );
	UnLock( lock );
	free( fib );

	/* Create info window */
	wi_Information = WindowObject,
		MUIA_Window_ID, 6,
		MUIA_Window_Title, GetCatStr( 70, "File Information" ),
		MUIA_Window_Menu, MUIV_Window_Menu_NoMenu,
		WindowContents, VGroup,
			Child, HGroup, MUIA_Group_SameSize, FALSE,
				TextFrame,
				MUIA_Background, MUII_TextBack,
				Child, tx_TextLeft = TextObject,
					TextFrame,
					MUIA_FramePhantomHoriz, TRUE,
					MUIA_Background, MUII_TextBack,
					MUIA_Text_Contents, GetCatStr( 71, "Name:\nDate:\nTime:\nSize:\nStatus:\nVersion:\nDesc:\nNote:" ),
					MUIA_Text_SetMax, TRUE,
					End,
				Child, tx_TextRight = TextObject,
					TextFrame,
					MUIA_FramePhantomHoriz, TRUE,
					MUIA_Background, MUII_TextBack,
					MUIA_Text_Contents, ctx->String,
					End,
				End,
			Child, HGroup,
				MUIA_Group_SameSize, TRUE,
				Child, bt_Okay = SimpleButton( GetCatStr( 72, "_Okay" ) ),
				Child, HSpace(0),
				Child, HSpace(0),
				Child, bt_Cancel = SimpleButton( GetCatStr( 73, "_Cancel" ) ),
				End,
			End,
		End;

	if ( !wi_Information )
		Fail();

	DoMethod( wi_Information, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2 );
	DoMethod( bt_Okay, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 1 );
	DoMethod( bt_Cancel, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2 );
	DoMethod( app_RumorOpus, OM_ADDMEMBER, wi_Information );

	set( wi_Information, MUIA_Window_Open, TRUE );
	set( wi_Information, MUIA_Window_ActiveObject, bt_Okay );

	success = TRUE;
	while ( success )
	{
		switch ( DoMethod( app_RumorOpus, MUIM_Application_Input, &signal ) )
		{
			case MUIV_Application_ReturnID_Quit:
				global_QuitProgram = QuitRequester();
				if ( global_QuitProgram )
				{
					ctx->Cancel = TRUE;
					success = FALSE;
				}
				break;
			case 1:
				success = FALSE;
				break;
			case 2:
				ctx->Cancel = TRUE;
				success = FALSE;
				break;
		}
		if ( signal ) Wait( signal );
	}

	set( wi_Information, MUIA_Window_Open, FALSE );
	DoMethod( app_RumorOpus, OM_REMMEMBER, wi_Information );
	MUI_DisposeObject( wi_Information );
}

/*
** Handler: listarc_entries
*/
static void HandleListArc( FuncContext * ctx )
{
	int TypeNum, rc;
	BOOL Request_Dst;

	if ( ctx->fib->fib_DirEntryType >= 0 )
		return;

	sprintf( Status_String, GetCatStr( 76, "Listing archive '%s'..." ), ctx->FileName_String );
	set( bt_StatusBar, MUIA_Text_Contents, Status_String );

	TypeNum = RecogArchive( ctx->FileName_String );
	if ( TypeNum == -1 )
	{
		if ( !ctx->NoDeselect_BOOL )
			DoMethod( lv_Directory[ctx->side], MUIM_List_Select, MUIV_List_Select_Active, MUIV_List_Select_Off, NULL );
		return;
	}

	strcpy( ctx->String, cfg_RecogCommand[TypeNum] );

	ParseMisc( ctx->Argument, &ctx->NoBreak_BOOL, &ctx->NoDeselect_BOOL );
	ParseReload( ctx->String, &ctx->ReloadSrc_BOOL, &ctx->ReloadDst_BOOL );
	ParseNeedTarget( ctx->side, ctx->String, &Request_Dst );

	if ( Request_Dst )
	{
		rc = ListArchive( ctx->String, ctx->FileName_String );
		if ( rc != 0 && !ctx->NoBreak_BOOL )
			ctx->ErrorNum = ERR_CMD_FAILED;
		ctx->Action_BOOL = TRUE;
	}
	else
		ctx->Cancel = TRUE;
}

/*
** Handler: protect_entries
*/
static void HandleProtect( FuncContext * ctx, BOOL * All_BOOL, LONG * prot_h, LONG * prot_s, LONG * prot_p, LONG * prot_a, LONG * prot_r, LONG * prot_w, LONG * prot_e, LONG * prot_d )
{
	APTR wi_Protection, bt_Okay, bt_Cancel, bt_All;
	APTR cy_Hide, cy_Script, cy_Pure, cy_Archive, cy_Read, cy_Write, cy_Execute, cy_Delete;
	__aligned struct FileInfoBlock * fib;
	BPTR lock;
	ULONG signal;
	BOOL success;
	const char * CY_Protect[] = { "X", "X", "X", NULL };
	char ProtBits[10];
	LONG Set_LONG;
	int l;

	sprintf( Status_String, GetCatStr( 78, "Changing protection bits for '%s'..." ), ctx->FileName_String );
	set( bt_StatusBar, MUIA_Text_Contents, Status_String );

	CY_Protect[0] = GetCatStr( 43, "Ignore" );
	CY_Protect[1] = GetCatStr( 44, "Set" );
	CY_Protect[2] = GetCatStr( 45, "UnSet" );

	if ( !*All_BOOL )
	{
		wi_Protection = WindowObject,
			MUIA_Window_ID, 7,
			MUIA_Window_Title, GetCatStr( 79, "Protection" ),
			MUIA_Window_Menu, MUIV_Window_Menu_NoMenu,
			WindowContents, VGroup,
				Child, HGroup, MUIA_Group_SameSize, TRUE,
					Child, ColGroup(2),
						Child, KeyLabel1( GetCatStr( 80, "Hide:" ), 'h' ), Child, cy_Hide = KeyCycle( CY_Protect, 'h' ),
						Child, KeyLabel1( GetCatStr( 81, "Script:" ), 's' ), Child, cy_Script = KeyCycle( CY_Protect, 's' ),
						Child, KeyLabel1( GetCatStr( 82, "Pure:" ), 'p' ), Child, cy_Pure = KeyCycle( CY_Protect, 'p' ),
						Child, KeyLabel1( GetCatStr( 83, "Archive:" ), 'a' ), Child, cy_Archive = KeyCycle( CY_Protect, 'a' ),
						End,
					Child, ColGroup(2),
						Child, KeyLabel1( GetCatStr( 84, "Read:" ), 'r' ), Child, cy_Read = KeyCycle( CY_Protect, 'r' ),
						Child, KeyLabel1( GetCatStr( 85, "Write:" ), 'w' ), Child, cy_Write = KeyCycle( CY_Protect, 'w' ),
						Child, KeyLabel1( GetCatStr( 86, "Execute:" ), 'e' ), Child, cy_Execute = KeyCycle( CY_Protect, 'e' ),
						Child, KeyLabel1( GetCatStr( 87, "Delete:" ), 'd' ), Child, cy_Delete = KeyCycle( CY_Protect, 'd' ),
						End,
					End,
				Child, HGroup, Child, HSpace(0), End,
				Child, HGroup,
					MUIA_Group_SameSize, TRUE,
					Child, bt_Okay = SimpleButton( GetCatStr( 72, "_Okay" ) ),
					Child, HSpace(0),
					Child, bt_All = SimpleButton( GetCatStr( 88, "A_ll" ) ),
					Child, HSpace(0),
					Child, bt_Cancel = SimpleButton( GetCatStr( 73, "_Cancel" ) ),
					End,
				End,
			End;

		if ( !wi_Protection )
			Fail();

		DoMethod( wi_Protection, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2 );
		DoMethod( bt_Okay, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 1 );
		DoMethod( bt_Cancel, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 2 );
		DoMethod( bt_All, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, 3 );
		DoMethod( app_RumorOpus, OM_ADDMEMBER, wi_Protection );

		set( wi_Protection, MUIA_Window_Open, TRUE );
		set( wi_Protection, MUIA_Window_ActiveObject, bt_Okay );

		success = TRUE;
		while ( success )
		{
			switch ( DoMethod( app_RumorOpus, MUIM_Application_Input, &signal ) )
			{
				case MUIV_Application_ReturnID_Quit:
					global_QuitProgram = QuitRequester();
					if ( global_QuitProgram )
					{
						ctx->Cancel = TRUE;
						success = FALSE;
					}
					break;
				case 1:
					success = FALSE;
					break;
				case 2:
					ctx->Cancel = TRUE;
					success = FALSE;
					break;
				case 3:
					success = FALSE;
					*All_BOOL = TRUE;
					break;
			}
			if ( signal ) Wait( signal );
		}

		set( wi_Protection, MUIA_Window_Open, FALSE );

		get( cy_Hide, MUIA_Cycle_Active, prot_h );
		get( cy_Script, MUIA_Cycle_Active, prot_s );
		get( cy_Pure, MUIA_Cycle_Active, prot_p );
		get( cy_Archive, MUIA_Cycle_Active, prot_a );
		get( cy_Read, MUIA_Cycle_Active, prot_r );
		get( cy_Write, MUIA_Cycle_Active, prot_w );
		get( cy_Execute, MUIA_Cycle_Active, prot_e );
		get( cy_Delete, MUIA_Cycle_Active, prot_d );

		DoMethod( app_RumorOpus, OM_REMMEMBER, wi_Protection );
		MUI_DisposeObject( wi_Protection );
	}

	if ( ctx->Cancel )
		return;

	fib = malloc( sizeof( struct FileInfoBlock ) );
	if ( !fib )
	{
		ctx->ErrorNum = ERR_NO_MEMORY;
		return;
	}

	lock = Lock( ctx->FileName_String, ACCESS_READ );
	if ( !lock )
	{
		ctx->ErrorNum = IoErr();
		free( fib );
		return;
	}

	Examine( lock, fib );

	for ( l = 0; l < 8; l++ )
		ProtBits[7-l] = ( fib->fib_Protection ^ 0x0f ) & ( 1 << l ) ? "dewrapsh"[l] : '-';
	ProtBits[8] = 0;

	Set_LONG = 0;
	if ( ProtBits[0] == 'h' ) Set_LONG += 128;
	if ( ProtBits[1] == 's' ) Set_LONG += 64;
	if ( ProtBits[2] == 'p' ) Set_LONG += 32;
	if ( ProtBits[3] == 'a' ) Set_LONG += 16;
	if ( ProtBits[4] == '-' ) Set_LONG += 8;
	if ( ProtBits[5] == '-' ) Set_LONG += 4;
	if ( ProtBits[6] == '-' ) Set_LONG += 2;
	if ( ProtBits[7] == '-' ) Set_LONG += 1;

	if ( *prot_h == 1 && ProtBits[0] == '-' ) Set_LONG += 128;
	if ( *prot_s == 1 && ProtBits[1] == '-' ) Set_LONG += 64;
	if ( *prot_p == 1 && ProtBits[2] == '-' ) Set_LONG += 32;
	if ( *prot_a == 1 && ProtBits[3] == '-' ) Set_LONG += 16;
	if ( *prot_r == 1 && ProtBits[4] != 'r' ) Set_LONG -= 8;
	if ( *prot_w == 1 && ProtBits[5] != 'w' ) Set_LONG -= 4;
	if ( *prot_e == 1 && ProtBits[6] != 'e' ) Set_LONG -= 2;
	if ( *prot_d == 1 && ProtBits[7] != 'd' ) Set_LONG -= 1;

	if ( *prot_h == 2 && ProtBits[0] == 'h' ) Set_LONG -= 128;
	if ( *prot_s == 2 && ProtBits[1] == 's' ) Set_LONG -= 64;
	if ( *prot_p == 2 && ProtBits[2] == 'p' ) Set_LONG -= 32;
	if ( *prot_a == 2 && ProtBits[3] == 'a' ) Set_LONG -= 16;
	if ( *prot_r == 2 && ProtBits[4] != '-' ) Set_LONG += 8;
	if ( *prot_w == 2 && ProtBits[5] != '-' ) Set_LONG += 4;
	if ( *prot_e == 2 && ProtBits[6] != '-' ) Set_LONG += 2;
	if ( *prot_d == 2 && ProtBits[7] != '-' ) Set_LONG += 1;

	success = SetProtection( ctx->FileName_String, Set_LONG );
	if ( success )
		ctx->ReloadSrc_BOOL = TRUE;
	else
		ctx->ErrorNum = IoErr();

	UnLock( lock );
	free( fib );
}

/*
**
** DoFunction() - Main entry point
**
*/

int DoFunction( int Operation, int side, char * Argument )
{
	FuncContext ctx;
	BPTR fh;
	__aligned struct FileInfoBlock * fib;
	ULONG i, k = 0, max, Entries_ULONG, Selection_State, Opened, Iconified;
	LONG Visible_LONG, Active_LONG, First_LONG, Diff_LONG, FirstSelected_LONG = -1, Over, DelForce;
	LONG prot_h = 0, prot_s = 0, prot_p = 0, prot_a = 0, prot_r = 0, prot_w = 0, prot_e = 0, prot_d = 0;
	char * ptrA = NULL, * NewName_String;
	BOOL Request_Dst = FALSE, Jumped = FALSE, All_BOOL = FALSE, CommentAll_BOOL = FALSE;
	char Version_String[81];

	/* Initialize context */
	ctx.side = side;
	ctx.Argument = Argument;
	ctx.ErrorNum = 0;
	ctx.Skip = FALSE;
	ctx.Cancel = FALSE;
	ctx.Action_BOOL = FALSE;
	ctx.ReloadSrc_BOOL = FALSE;
	ctx.ReloadDst_BOOL = FALSE;
	ctx.NoBreak_BOOL = FALSE;
	ctx.NoDeselect_BOOL = FALSE;
	strcpy( ctx.String, "" );

	if ( Argument != NULL )
	{
		ParseMisc( Argument, &ctx.NoBreak_BOOL, &ctx.NoDeselect_BOOL );
		ParseReload( Argument, &ctx.ReloadSrc_BOOL, &ctx.ReloadDst_BOOL );
		ParseNeedTarget( side, Argument, &Request_Dst );
	}

	if ( Operation == command_entries && Request_Dst )
	{
		ptrA = strstr( Argument, "%f" );
		if ( ptrA == NULL )
			ptrA = strstr( Argument, "%F" );
		if ( ptrA == NULL )
			ptrA = strstr( Argument, "%n" );
		if ( ptrA == NULL )
			ptrA = strstr( Argument, "%N" );

		if ( ptrA == NULL )
		{
			int rc = ExecuteCommand( Argument, "" );
			if ( rc != 0 && !ctx.NoBreak_BOOL )
				ctx.ErrorNum = ERR_CMD_FAILED;
		}
	}

	if ( ( Operation != command_entries ) || ( ptrA != NULL ) )
	{
		if ( global_DirLoaded[side] )
		{
			Over = cfg_Overwrite;
			DelForce = cfg_DelForce;

			get( lv_Directory[side], MUIA_List_Active, &Active_LONG );
			get( lv_Directory[side], MUIA_List_Entries, &Entries_ULONG );

			for ( i = 0; i < Entries_ULONG; i++ )
			{
				DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );
				if ( Selection_State == MUIV_List_Select_On )
				{
					k++;
					if ( FirstSelected_LONG == -1 )
						FirstSelected_LONG = i;
				}
			}

			if ( k == 0 && Active_LONG != MUIV_List_Active_Off )
			{
				k = 1;
				DoMethod( lv_Directory[side], MUIM_List_Select, Active_LONG, MUIV_List_Select_On, NULL );
			}

			set( ga_Gauge, MUIA_Gauge_Current, 0 );
			set( ga_Gauge, MUIA_Gauge_Max, k );

			if ( k > 0 )
			{
				k = 0;
				set( wi_Progress, MUIA_Window_Open, TRUE );

				if ( cfg_Scroll )
				{
					get( lv_Directory[side], MUIA_List_Visible, &Visible_LONG );
					get( lv_Directory[side], MUIA_List_First, &First_LONG );

					if ( FirstSelected_LONG >= First_LONG )
						Diff_LONG = FirstSelected_LONG - First_LONG;
					else
					{
						Diff_LONG = Visible_LONG + 1;
						First_LONG = FirstSelected_LONG;
					}

					if ( Diff_LONG > Visible_LONG )
					{
						set( lv_Directory[side], MUIA_List_Quiet, TRUE );
						DoMethod( lv_Directory[side], MUIM_List_Jump, Entries_ULONG - 1 );
						DoMethod( lv_Directory[side], MUIM_List_Jump, FirstSelected_LONG );
					}
				}

				for ( i = 0; i < Entries_ULONG; i++ )
				{
					if ( cfg_Scroll && ( ( ( i - First_LONG ) % Visible_LONG ) == 0 ) && i > First_LONG )
					{
						set( lv_Directory[side], MUIA_List_Quiet, TRUE );
						if ( ( i + Visible_LONG ) < Entries_ULONG )
							DoMethod( lv_Directory[side], MUIM_List_Jump, i - 1 + Visible_LONG );
						else
							DoMethod( lv_Directory[side], MUIM_List_Jump, Entries_ULONG - 1 );
						Jumped = TRUE;
					}

					DoMethod( app_RumorOpus, MUIM_Application_InputBuffered );
					DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Ask, &Selection_State );

					if ( Selection_State == MUIV_List_Select_On )
					{
						if ( cfg_Scroll )
							set( lv_Directory[side], MUIA_List_Quiet, FALSE );

						get( wi_Progress, MUIA_Window_Open, &Opened );
						get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
						if ( !Opened && !Iconified )
							break;

						DoMethod( lv_Directory[side], MUIM_List_GetEntry, i, &fib );
						ctx.fib = fib;
						sprintf( ctx.FileName_String, "%s%s", GetPath( side ), fib->fib_FileName );
						strcpy( ctx.String, "" );

						switch ( Operation )
						{
							case command_entries:
								HandleCommand( &ctx, Request_Dst );
								break;

							case action_entries:
								HandleAction( &ctx );
								break;

							case copydev_entries:
								HandleCopyDev( &ctx );
								break;

							case delete_entries:
								HandleDelete( &ctx );
								break;

							case unarc_entries:
								HandleUnarc( &ctx );
								break;

							case drop_entries:
							case copy_entries:
							case move_entries:
							case copyas_entries:
							case moveas_entries:
								HandleCopyMove( &ctx, Operation );
								break;

							case rename_entries:
								HandleRename( &ctx );
								break;

							case dup_entries:
								HandleDuplicate( &ctx );
								break;

							case touch_entries:
								HandleTouch( &ctx );
								break;

							case comment_entries:
								HandleComment( &ctx, Version_String, &CommentAll_BOOL );
								break;

							case setdate_entries:
								HandleSetDate( &ctx );
								break;

							case info_entries:
								HandleInfo( &ctx );
								break;

							case listarc_entries:
								HandleListArc( &ctx );
								break;

							case protect_entries:
								HandleProtect( &ctx, &All_BOOL, &prot_h, &prot_s, &prot_p, &prot_a, &prot_r, &prot_w, &prot_e, &prot_d );
								break;
						}

						if ( ctx.Cancel || ctx.ErrorNum != 0 )
							break;

						ctx.Action_BOOL = TRUE;

						if ( !ctx.NoDeselect_BOOL )
							DoMethod( lv_Directory[side], MUIM_List_Select, i, MUIV_List_Select_Toggle, NULL );

						k++;
						set( ga_Gauge, MUIA_Gauge_Current, k );
					}

					get( ga_Gauge, MUIA_Gauge_Max, &max );
					if ( max == k )
						break;
				}

				set( wi_Progress, MUIA_Window_Open, FALSE );

				if ( ctx.Action_BOOL )
				{
					if ( ctx.ReloadSrc_BOOL )
						Reload( side );
					else
					{
						if ( cfg_Scroll && Jumped )
						{
							set( lv_Directory[side], MUIA_List_Quiet, TRUE );
							DoMethod( lv_Directory[side], MUIM_List_Jump, Entries_ULONG - 1 );
							Active_LONG = Active_LONG - ( Visible_LONG / 2 );
							if ( Active_LONG < 0 )
								Active_LONG = 0;
							DoMethod( lv_Directory[side], MUIM_List_Jump, Active_LONG );
						}
						UpdateNumFiles( side );
					}

					if ( ctx.ReloadDst_BOOL && global_DirLoaded[OtherSide(side)] )
						Reload( OtherSide( side ) );
				}

				if ( cfg_Scroll )
					set( lv_Directory[side], MUIA_List_Quiet, FALSE );
			}

			/* Handle touch for new file creation */
			if ( !ctx.Action_BOOL && ( Operation == touch_entries ) )
			{
				NewName_String = StringRequester( GetCatStr( 89, "Enter file name" ), "", ":/", 31, 0, &ctx.Skip, &ctx.Cancel );
				if ( !ctx.Cancel && strlen( NewName_String ) > 0 )
				{
					sprintf( ctx.Target_String, "%s%s", GetPath( side ), NewName_String );
					if ( Exists( ctx.Target_String ) )
						if ( Overwrite( ctx.FileName_String, ctx.Target_String ) == 1 )
							ctx.ErrorNum = Delete( ctx.Target_String );

					if ( ctx.ErrorNum == 0 )
					{
						fh = Open( ctx.Target_String, MODE_NEWFILE );
						if ( fh )
						{
							Close( fh );
							if ( cfg_CreateIcons && ( strlen( FilePart( ctx.Target_String ) ) + 5 ) < 31 )
							{
								sprintf( ctx.String, "%s.info", ctx.Target_String );
								ctx.ErrorNum = CopyFile( "ENV:sys/def_project.info", ctx.String, FALSE );
							}
							Reload( side );
						}
						else
							ctx.ErrorNum = IoErr();
					}
				}
			}

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

			cfg_Overwrite = Over;
			cfg_DelForce = DelForce;
		}
	}

	global_AskOnce = FALSE;
	return ctx.ErrorNum;
}
