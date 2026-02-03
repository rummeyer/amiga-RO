#include "Includes.h"
#include "Misc.h"

/*
**
** CheckMemory()
**
*/

BOOL CheckMemory ( int Needed_Memory )
{
	char * Memory = malloc( Needed_Memory );

	if ( Memory )
		free( Memory );

	return Memory ? TRUE : FALSE;
}

/*
**
** OtherSide()
**
*/

int OtherSide ( int side )
{
	return ( side == Left_Side ) ? Right_Side : Left_Side;
}

/*
**
** MouseSide()
**
*/

int MouseSide ( int X, int Y )
{
	LONG Top, Bottom, Width, Height, Left, Right;
	int ret = -1;

	get( lv_Directory[Left_Side], MUIA_TopEdge, &Top );
	get( lv_Directory[Left_Side], MUIA_BottomEdge, &Bottom );
	get( lv_Directory[Right_Side], MUIA_LeftEdge, &Left );
	get( lv_Directory[Left_Side], MUIA_RightEdge, &Right );
	get( wi_Main, MUIA_Window_Width, &Width );
	get( wi_Main, MUIA_Window_Height, &Height );

	if ( ( Y > Top ) && ( Y < Bottom ) && ( X > 0 ) && ( X < Width ) )
	{
		if ( global_Unfolded )
		{
			if ( X < Right )
				ret = Left_Side;
			if ( X > Left )
				ret = Right_Side;
		}
		else
			ret = Left_Side;
	}

    if ( ( Y > Bottom ) && ( Y < Height ) && ( X > 0 ) && ( X < Width ) )
		ret = -2;

	return ( ret );
}

/*
**
** StringSide()
**
*/

int StringSide ( char * cptr )
{
	char Side_String[10];
	int side = -1;

	strcpy( Side_String, cptr );
	if ( stricmp( Side_String, "left" ) == 0 )
		side = Left_Side;
	if ( stricmp( Side_String, "right" ) == 0 )
		side = Right_Side;
	if ( stricmp( Side_String, "active" ) == 0 )
		side = Active_Side;
	if ( stricmp( Side_String, "inactive" ) == 0 )
		side = OtherSide( Active_Side );

	return ( side );
}

/*
**
** SleepWindow()
**
*/

void SleepWindow( BOOL Sleep_BOOL )
{
	set( wi_Main, MUIA_Window_Sleep, Sleep_BOOL );
}

/*
**
** SleepClock()
**
*/

void SleepClock( BOOL Sleep_BOOL )
{

	if ( !Sleep_BOOL )
	{
		if ( global_SleepClock > 0 )
		{
			global_SleepClock--;
			if ( global_SleepClock == 0 )
			{
				if ( cfg_ShowClock || cfg_ShowDate || cfg_ShowDay || cfg_ShowMem )
					UpdateClock();
				else
					set( bt_StatusBar, MUIA_Text_Contents, global_IdleStatus );
			}
		}
	}
	else
		global_SleepClock++;
}


/*
**
** Sleep()
**
*/

void Sleep( BOOL Sleep_BOOL )
{
	SleepWindow( Sleep_BOOL );
	SleepClock( Sleep_BOOL );
}

/*
**
** ActivateList()
**
*/

void ActivateList ( int side )
{
	LONG ActivePage_LONG;

	get( pg_Page[side], MUIA_Group_ActivePage, &ActivePage_LONG );
	if ( ActivePage_LONG == 0) set( wi_Main, MUIA_Window_ActiveObject, lv_Directory[side] );
	SetActiveColor( side );
}

/*
**
** SetActiveColor()
**
*/

void SetActiveColor ( int side )
{
	LONG PageFront[2];

	get( pg_Page[Right_Side], MUIA_Group_ActivePage, &PageFront[Right_Side] );
	get( pg_Page[Left_Side], MUIA_Group_ActivePage, &PageFront[Left_Side] );
	if ( PageFront[side] == 0 )
		set( bt_StatusText[side], MUIA_Background, MUII_ListSelect );
	if ( PageFront[OtherSide(side)] == 0 )
		set( bt_StatusText[OtherSide(side)], MUIA_Background, MUII_TextBack );
	Active_Side = side;
}

/*
**
** GetCatStr()
**
*/

char * GetCatStr( int num, char * defstring )
{
	return global_Catalog ? GetCatalogStr( global_Catalog, num, defstring ) : defstring;
}

/*
**
** NumberToString()
**
*/

char * NumberToString( ULONG num )
{
	char Source[21], Target[21];
	int i,j,len,k=0;

	sprintf( Source, "%d", num );
	len = strlen( Source );
	if( len > 3 )
	{
		j = len + 1;
		if( len > 6 ) j++;
		if( len > 9 ) j++;
		Target[j] = '\0';
		for( i = len - 1; i >= 0; i-- )
		{
			j--; k++;
			Target[j] = Source[i];
			if( (k == 3) && ( j - 1 > 0 ) )
			{
				k=0; j--;
				Target[j] = global_GroupSeparator[0];
			}
		}
		sprintf( Source, "%s", Target );
	}
	return( strdup ( Source ) );
}

/*
**
** Error()
**
*/

char * Error( int error_number )
{
	char Error_String[81], Return_String[256];

	if ( error_number > 0 )
		Fault( error_number, GetCatStr( 27, "Last command failed because" ), Error_String, 80 );
	else
	{
		strcpy(Error_String, GetCatStr( 27, "Last command failed because" ) );
		strcat(Error_String, ": ");

		switch ( error_number )
		{
			case ERR_NO_MEMORY:
				strcat(Error_String, GetCatStr( 28, "not enough memory available" ) );
				break;
			case ERR_DATE_TO_STR:
				strcat(Error_String, GetCatStr( 29, "unable to convert datestamp to string" ) );
				break;
			case ERR_STR_TO_DATE:
				strcat(Error_String, GetCatStr( 30, "unable to convert string to date" ) );
				break;
			case ERR_CMD_FAILED:
				strcat(Error_String, GetCatStr( 31, "unexpected command termination" ) );
				break;
			case ERR_INFINITE_LOOP:
				strcat(Error_String, GetCatStr( 32, "infinite loop" ) );
				break;
			case ERR_CANT_DELETE_SRC:
				strcat(Error_String, GetCatStr( 33, "source may not be deleted" ) );
				break;
			case ERR_FILE_EXISTS:
				strcat(Error_String, GetCatStr( 34, "file already exists" ) );
				break;
			case ERR_NO_XPK_LIB:
				strcat(Error_String, GetCatStr( 35, "unable to open 'xpkmaster.library'" ) );
				break;
			case ERR_UNPACK_FAILED:
				strcat(Error_String, GetCatStr( 36, "unable to unpack file" ) );
				break;
			case ERR_NO_DISK_SPACE:
				strcat(Error_String, GetCatStr( 37, "not enough disk space" ) );
				break;
			case ERR_EXPAND_PATH:
				strcat(Error_String, GetCatStr( 38, "unable to expand path" ) );
				break;
			case ERR_ABORTED:
				strcat(Error_String, GetCatStr( 122, "function aborted" ) );
				break;
			case ERR_VERIFY:
				strcat(Error_String, GetCatStr( 123, "verify error" ) );
				break;
			case ERR_UNSUPPORTED:
				strcat(Error_String, GetCatStr( 127, "unsupported built-in function called" ) );
				break;
		}

	}
	strcpy( Return_String, Error_String );
	return( Return_String );
}

/*
**
** ParseCommand()
**
*/

char * ParseCommand( char * Argument_String, char * File_Name, BOOL * Wait_BOOL, BOOL * CD_Source, BOOL * CD_Dest )
{
	int i, j = 0;
	char * cptr, New_String[512], Old_String[512], String[256];
	char Source_Path[256], Target_Path[256], Source_Device[41], Target_Device[41];
	BOOL Cancel = FALSE, Skip;

	* Wait_BOOL = FALSE;
	* CD_Source = FALSE;
	* CD_Dest = FALSE;

	strcpy( Source_Path, "" );
	strcpy( Target_Path, "" );
	strcpy( Source_Device, "" );
	strcpy( Target_Device, "" );
	strcpy( New_String, "" );
	strcpy( Old_String, "" );

	cptr = strstr ( Argument_String, "Rx " );
	if ( cptr == NULL )
	{
		cptr = strstr ( Argument_String, "rx " );
		if ( cptr == NULL )
		{
			cptr = strstr ( Argument_String, "rX " );
			if ( cptr == NULL )
				cptr = strstr ( Argument_String, "RX " );
		}
	}

	if ( cptr != NULL )
		strcat( Old_String, "Run >NIL: " );

	strcat( Old_String, Argument_String );

	if ( global_DirLoaded[Active_Side] )
	{
		strcpy( Source_Path, GetPath( Active_Side ) );
		strcpy( String, Source_Path );
		cptr = strrchr( String, ':' );
		cptr++;
		* cptr = '\0';
		strcpy( Source_Device, String );
	}

	if ( global_DirLoaded[OtherSide(Active_Side)] )
	{
		strcpy( Target_Path, GetPath( OtherSide( Active_Side ) ) );
		strcpy( String, Target_Path );
		cptr = strrchr( String, ':' );
		cptr++;
		* cptr = '\0';
		strcpy( Target_Device, String );
	}

	for( i = 0; i <= strlen( Old_String ); i++ )
	{
		if ( ( Old_String[i] != '%' ) && ( Old_String[i] != '&' ) )
		{
			New_String[j] = Old_String[i];
			j++;
			New_String[j] = '\0';
		}
		else
		{
			switch( Old_String[i] )
			{

				case '&' :
					i++;
					switch(Old_String[i])
					{
						case 'w' :	* Wait_BOOL = TRUE;	break;
						case 'c' :	* CD_Source = TRUE;	break;
						case 'C' :	* CD_Dest = TRUE;	break;
						case '&' :	strcat( New_String, "&" );
									j++;
									break;
					}
					break;

				case '%' :
					i++;
					switch(Old_String[i])
					{
						case '%' :	strcat( New_String, "%" );
									break;

						case 'A' :
						case 'a' :	strcat( New_String, global_ARexxName );
									break;

						case 'f' :	if ( strlen( File_Name ) > 0 )
									{
										sprintf( String, "\"%s\"", File_Name );
										strcat( New_String, String );
									}
									break;

						case 'F' :	if ( strlen( File_Name ) > 0 )
										strcat( New_String, File_Name );
									break;

						case 'n' :	if ( strlen( File_Name ) > 0 )
									{
										sprintf( String, "\"%s\"", FilePart( File_Name ) );
										strcat( New_String, String );
									}
									break;

						case 'N' :	if ( strlen( File_Name ) > 0 )
										strcat( New_String, FilePart( File_Name ) );
									break;

						case 'p' :	sprintf( String, "\"%s\"", Source_Path );
									strcat( New_String, String );
									break;

						case 'P' :	strcat(	New_String, Source_Path );
									break;

						case 't' :	sprintf( String, "\"%s\"", Target_Path );
									strcat( New_String, String );
									break;

						case 'T' :	strcat( New_String, Target_Path );
									break;

						case 's' :	sprintf( String, "\"%s\"", Source_Path );
									strcat( New_String, String );
									break;

						case 'S' :	strcat(	New_String, Source_Path );
									break;

						case 'd' :	sprintf( String, "\"%s\"", Target_Path );
									strcat( New_String, String );
									break;

						case 'D' :	strcat( New_String, Target_Path );
									break;

						case 'r' :	strcpy( String, StringRequester( GetCatStr( 75, "Input Request" ), global_Exec_String, "", 256, 0, &Skip, &Cancel ) );
									strcat( New_String, String );
									if ( strlen ( String ) > 0 )
										strcpy( global_Exec_String, String );
									break;

						case 'R' :	if ( !global_AskOnce )
									{
										strcpy( String, StringRequester( GetCatStr( 75, "Input Request" ), global_Exec_String, "", 256, 0, &Skip, &Cancel ) );
										global_AskOnce = TRUE;
										strcat( New_String, String );
										if ( strlen ( String ) > 0 )
											strcpy( global_Exec_String, String );
									}
									else
										strcat( New_String, global_Exec_String );
									break;
					}
					j = strlen( New_String );
					break;
			}
		}
	}

	if ( Cancel )
		strcpy( New_String, "" );

	return( New_String );
}

/*
**
** ParseReload()
**
*/

void ParseReload( char * Argument, BOOL * ReloadSrc_BOOL, BOOL * ReloadDst_BOOL )
{
	char * ptrB;

	* ReloadSrc_BOOL = FALSE;
	* ReloadDst_BOOL = FALSE;

	if ( Argument != NULL )
	{
		ptrB = strstr( Argument, "&r" );
		if ( ptrB != NULL )
		{
			* ReloadSrc_BOOL = TRUE;
		}
		ptrB = strstr( Argument, "&R" );
		if ( ptrB != NULL )
			* ReloadDst_BOOL = TRUE;
	}
}

/*
**
** ParseMisc()
**
*/

void ParseMisc( char * Argument, BOOL * NoBreak_BOOL, BOOL * NoDeselect_BOOL )
{
	char * ptrB;

	* NoBreak_BOOL = FALSE;
	* NoDeselect_BOOL = FALSE;

	if ( Argument != NULL )
	{
		ptrB = strstr( Argument, "&b" );
		if ( ptrB != NULL )
		{
			* NoBreak_BOOL = TRUE;
		}
		ptrB = strstr( Argument, "&d" );
		if ( ptrB != NULL )
			* NoDeselect_BOOL = TRUE;
	}
}


/*
**
** ParseNeedTarget()
**
*/

void ParseNeedTarget( int side, char * Argument, BOOL * Request_Dst )
{
	char * ptrB = NULL;

	* Request_Dst = TRUE;

	if ( Argument != NULL )
		ptrB = strstr( Argument, "%T" );
		if ( ptrB == NULL )
			ptrB = strstr( Argument, "%t" );
			if ( ptrB == NULL )
				ptrB = strstr( Argument, "%D" );
				if ( ptrB == NULL )
					ptrB = strstr( Argument, "%d" );
						if ( ptrB == NULL )
							ptrB = strstr( Argument, "&C" );

	if ( ptrB != NULL )
		* Request_Dst = global_DirLoaded[OtherSide(side)];
}

/*
**
** GetVersion()
**
*/

char * GetVersion ( char * FileName_String )
{
	FILE * file;
	char * VerString = "$VER: ", String[81];
	BOOL found = FALSE;
	int i = 0, j = 0, c;

	strcpy( String, GetCatStr( 126, "unknown" ) );

	if ( Exists ( FileName_String ) )
	{
		file = fopen( FileName_String, "rb" );
		if ( file )
		{
			while( ( c = fgetc( file ) ) != EOF )
			{
				if ( found )
				{
					if ( c == 0 || c == 10 || c == 13 )
					{
						String[j] = 0;
						break;
					}
					else
					{
						String[j]=c;
						if ( j < 80 )
							j++;
					}
				}

				if ( c == VerString[i] && !found )
				{
					i++;
					if ( i == 6 )
						found = TRUE;
				}
				else
					i = 0;
			}
			fclose( file );
		}
	}

	return( String );
}

/*
** MatchFileType() - Helper for Recog/RecogArchive
**
** Matches files against configured patterns (hex signatures + filename patterns)
** ArchiveMode: TRUE = only check entries with extraction commands (archives)
**              FALSE = only check entries without extraction commands (regular files)
** Returns index into cfg_Recog* arrays, or -1 if no match
*/

static int MatchFileType ( char * FileName_String, char * Buffer, int BufferLen, BOOL ArchiveMode )
{
	char * Pattern_Token;
	int i, j, f = 0;
	BOOL Found_Hex = FALSE, Found_Name = FALSE;

	for ( i = 0; i < 80; i++ )
	{
		/* ArchiveMode: check entries WITH CommandB, else entries WITHOUT */
		if ( ArchiveMode ? ( strlen( cfg_RecogCommandB[i] ) > 0 ) : ( strlen( cfg_RecogCommandB[i] ) == 0 ) )
		{
			if ( strlen( cfg_RecogHex[i] ) > 0 )
			{
				if ( strlen( cfg_RecogHex[i] ) <= BufferLen )
				{
					for ( j = 0; ( j < strlen( cfg_RecogHex[i] ) ) && ( j < BufferLen ); j++ )
						if ( cfg_RecogHex[i][j] == Buffer[j] || cfg_RecogHex[i][j] == '?' || ( cfg_RecogHex[i][j] == 255 && Buffer[j] == 0 ) )
							f++;
					if ( f == strlen( cfg_RecogHex[i] ) )
						Found_Hex = TRUE;
				}
			}
			else
				Found_Hex = TRUE;

			if ( Found_Hex )
			{
				Pattern_Token = malloc( 256 );
				if ( Pattern_Token )
				{
					ParsePatternNoCase( cfg_RecogString[i], Pattern_Token, 256 );
					if ( MatchPatternNoCase( Pattern_Token, FilePart( FileName_String ) ) )
						Found_Name = TRUE;
					free( Pattern_Token );
				}
			}

			if ( Found_Hex && Found_Name )
				return i;

			Found_Hex = FALSE;
			Found_Name = FALSE;
			f = 0;
		}
	}

	return -1;
}

/*
** Recog() - Recognize regular (non-archive) filetypes
**
** Returns:
**   -1 = unknown binary
**   -2 = ASCII text
**   -3 = Amiga executable
**   -4 = XPK/PowerPacker compressed (skip further matching)
**   >=0 = index into cfg_Recog* configuration arrays
*/

int Recog ( char * FileName_String )
{
	char Buffer[80];
	int j, count = 0, k, Num = -1, Result;
	FILE * file;

	if ( !Exists( FileName_String ) )
		return -1;

	file = fopen( FileName_String, "rb" );
	if ( !file )
		return -1;

	k = fread( Buffer, 1, 80, file );
	fclose( file );

	if ( k == 0 )
		return -1;

	/* Check for ASCII text */
	for ( j = 0; j < k; j++ )
		if ( ( Buffer[j] > 31 && Buffer[j] < 128 ) || Buffer[j] > 159 || ( Buffer[j] > 8 && Buffer[j] < 14 ) )
			count++;
	if ( count == k )
		Num = -2;

	/* Check for Amiga executable */
	if ( Buffer[0] == 0 && Buffer[1] == 0 && Buffer[2] == 3 && Buffer[3] == 243 &&
	     Buffer[4] == 0 && Buffer[5] == 0 && Buffer[6] == 0 && Buffer[7] == 0 )
		Num = -3;

	/* Check for XPK/PowerPacker */
	if ( ( Buffer[0] == 88 && Buffer[1] == 80 && Buffer[2] == 75 && Buffer[3] == 70 && Buffer[4] == 0 ) ||
	     ( Buffer[0] == 80 && Buffer[1] == 80 && Buffer[2] == 50 && Buffer[3] == 48 && Buffer[4] == 9 && Buffer[5] == 10 ) )
		Num = -4;

	/* Skip filetype matching for XPK/PP files */
	if ( Num == -4 )
		return Num;

	Result = MatchFileType( FileName_String, Buffer, k, FALSE );
	return ( Result >= 0 ) ? Result : Num;
}

/*
** RecogArchive() - Recognize archive filetypes (files with extraction commands)
**
** Returns index into cfg_Recog* arrays for archives, or -1 if no match
*/

int RecogArchive ( char * FileName_String )
{
	char Buffer[80];
	int k;
	FILE * file;

	if ( !Exists( FileName_String ) )
		return -1;

	file = fopen( FileName_String, "rb" );
	if ( !file )
		return -1;

	k = fread( Buffer, 1, 80, file );
	fclose( file );

	if ( k == 0 )
		return -1;

	return MatchFileType( FileName_String, Buffer, k, TRUE );
}
