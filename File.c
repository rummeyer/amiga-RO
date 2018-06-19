#include "Includes.h"
#include "File.h"

/*
**
** Exists()
**
*/

BOOL Exists( char * File_String )
{
	BPTR lock;
	
	lock = Lock( File_String, ACCESS_READ );
	if ( lock )
	{
		UnLock( lock );
		return( TRUE );
	}
	else
		return( FALSE );
}

/*
**
** RemoveFile()
**
*/

int RemoveFile ( char * File_String )
{
	char Text_String[1024];
	int ErrorNum = 0, num;
	LONG Opened, Iconified;
	BOOL UnProt_BOOL = FALSE, success;

	DoMethod( app_RumorOpus, MUIM_Application_InputBuffered );
	get( wi_Progress, MUIA_Window_Open, &Opened );
	get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
	if ( Opened || Iconified )
	{
		success = DeleteFile( File_String ); 
		if ( !success )
		{
			ErrorNum = IoErr();
			if ( ErrorNum == ERROR_DELETE_PROTECTED )
			{
				ErrorNum = 0;

				if ( cfg_DelForce == 0 )
				{
					sprintf( Text_String, GetCatStr( 90, "File %s is protected from deletion.\nDo you wish to unprotect it?" ), File_String );
					num = MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 50, "Request" ), GetCatStr( 91, "*_Okay|_All|_Skip|_None|_Cancel" ), Text_String, TAG_END );
	
					switch ( num )
					{
						case 1 :	UnProt_BOOL = TRUE;					break;
						case 2 :	cfg_DelForce = 1;					break;
						case 3 :	UnProt_BOOL = FALSE;				break;
						case 4 :	cfg_DelForce = 2;					break;
						case 0 :	ErrorNum = ERROR_DELETE_PROTECTED;	break;
					}
				}

				if ( UnProt_BOOL || cfg_DelForce == 1 )
				{
					success = SetProtection( File_String, 0 );
					if ( !success )
						ErrorNum = IoErr();
					else
					{
						success = DeleteFile( File_String );
						if ( !success )
							ErrorNum = IoErr();
						else
							ErrorNum = 0;
					}
				}
				else
					if ( ErrorNum == 0 )
						ErrorNum = -20;
			}
		}
	}
	else
		ErrorNum = -12;

	return( ErrorNum );
}

/*
**
** RemoveAFile()
**
*/

int RemoveAFile ( char * File_String )
{
	char Text_String[1024];
	int ErrorNum = 0, num;
	LONG Opened, Iconified;
	BOOL UnProt_BOOL = FALSE, success;

	DoMethod( app_RumorOpus, MUIM_Application_InputBuffered );
	get( wi_Progress, MUIA_Window_Open, &Opened );
	get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
	if ( Opened || Iconified )
	{
		success = DeleteFile( File_String ); 
		if ( !success )
		{
			ErrorNum = IoErr();
			if ( ErrorNum == ERROR_DELETE_PROTECTED )
			{
				if ( cfg_DelForce == 0 )
				{
					sprintf( Text_String, GetCatStr( 90, "File %s is protected from deletion.\nDo you wish to unprotect it?" ), File_String );
					num = MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 50, "Request" ), GetCatStr( 125, "*_Okay|_All|_Cancel" ), Text_String, TAG_END );
	
					switch ( num )
					{
						case 1 :	UnProt_BOOL = TRUE;					break;
						case 2 :	cfg_DelForce = 1;					break;
						case 0 :	ErrorNum = ERROR_DELETE_PROTECTED;	break;
					}
				}

				if ( UnProt_BOOL || cfg_DelForce == 1 )
				{
					success = SetProtection( File_String, 0 );
					if ( !success )
						ErrorNum = IoErr();
					else
					{
						success = DeleteFile( File_String );
						if ( !success )
							ErrorNum = IoErr();
						else
							ErrorNum = 0;
					}
				}
			}
		}
	}
	else
		ErrorNum = -12;

	return( ErrorNum );
}


/*
**
** Overwrite()
**
*/

int Overwrite ( char * Source_String, char * File_String )
{
	char Text_String[512];
	int Over = 0;
	LONG Result_LONG;
	int num;
	BPTR srclock, dstlock;
	__aligned struct FileInfoBlock * srcfib;
	__aligned struct FileInfoBlock * dstfib;

	if ( cfg_Overwrite == 2 )
	{
		sprintf( Text_String, GetCatStr( 92, "File %s already exists.\nDo you wish to replace it?" ), File_String );
		num = MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 50, "Request" ), GetCatStr( 91, "*_Okay|_All|_Skip|_None|_Cancel" ), Text_String, TAG_END );

		switch ( num )
		{
			case 1 :	Over = 1;			break;
			case 2 :	cfg_Overwrite = 0;	break;
			case 3 :	Over = 0;			break;
			case 4 :    cfg_Overwrite = 4;	break;
			case 0 :	Over = -1;			break;
		}
	}

	if ( cfg_Overwrite == 0 )
		Over = 1;

	if ( cfg_Overwrite == 1 )
		Over = 0;

	if ( cfg_Overwrite == 4 )
		Over = 0;

	if ( cfg_Overwrite == 3 )
	{
		srcfib = malloc( sizeof( struct FileInfoBlock ) );
		if ( srcfib )
		{
			dstfib = malloc( sizeof( struct FileInfoBlock ) );
			if ( dstfib )
			{
				srclock = Lock( Source_String, ACCESS_READ );
				if ( srclock )
				{
					dstlock = Lock( File_String, ACCESS_READ );
					if ( dstlock )
					{
						Examine( srclock, srcfib );
						Examine( dstlock, dstfib );
						Result_LONG = CompareDates( &srcfib -> fib_Date, &dstfib -> fib_Date );
						if ( Result_LONG >= 0 )
							Over = 0;
						else
							Over = 1;
						UnLock( dstlock );
					}
					UnLock( srclock );
				}
				free( dstfib );
			}
			free( srcfib );
		}
	}

	return( Over );
}

/*
**
** Clone()
**
*/

BOOL Clone ( char * Source_String, char * Target_String )
{
	BOOL success = FALSE;
	BPTR lock;
	__aligned struct FileInfoBlock * fib;
	struct DateStamp * date;

	lock = Lock( Source_String, ACCESS_READ );
	if ( lock )
	{
		fib = malloc( sizeof( struct FileInfoBlock ) );
		if ( fib )
		{
			Examine( lock, fib );

			if ( cfg_CopyDate )
			{
				date = malloc( sizeof( struct DateStamp ) );
				if ( date )
				{
					*date = fib -> fib_Date;
					SetFileDate( Target_String, date );
					free( date );
				}
			}

			if ( cfg_CopyNote )
				SetComment( Target_String, fib -> fib_Comment );

			if ( cfg_CopyFlags )
				SetProtection( Target_String, fib -> fib_Protection );
			else
				SetProtection( Target_String, 0 );

			success = TRUE;
			free( fib );
		}
		UnLock( lock );
	}
	else
		success = FALSE;
	return( success );
}

/*
**
** CopyFile()
**
*/

int CopyFile ( char * Source, char * Target, BOOL Progress )
{
	int ErrorNum = 0;
	BPTR lock;
	FILE * Source_File, * Target_File;
	int Proceed, Percent = 100;
	LONG AvailMem_LONG, MemSize_LONG, size_src, size_dst, pos=0, i, Opened=TRUE, Iconified=FALSE;
	__aligned struct FileInfoBlock * fib;
	char * Buffer[2], Source_String[256], Target_String[256], Cat_String[81];

	strcpy( Source_String, Source );
	strcpy( Target_String, Target );

	if ( cfg_CopyStat && Progress )
	{
		strcpy( Cat_String, GetCatStr( 93, "Copying '%s' to '%s'... (%s%% done)" ) );
		sprintf( Status_String, Cat_String, Source_String, Target_String, "0" );
		set( bt_StatusBar, MUIA_Text_Contents, Status_String );
	}

	lock = Lock( Source_String, ACCESS_READ );
	if ( lock )
	{
		if ( Exists( Target_String ) )
		{
			Proceed = Overwrite( Source_String, Target_String );
			if ( Proceed == 1 )
				ErrorNum = Delete( Target_String );
			if ( Proceed == 0 )
			{
				ErrorNum = -20;
				if ( cfg_Overwrite == 1 )
					ErrorNum = -7;
			}
		}
		else
			Proceed = 1;

		if ( Proceed == 1 && ErrorNum == 0 )
		{
			Source_File = fopen( Source_String, "r" );
			if ( Source_File )
			{
				Target_File = fopen( Target_String, "a+" );
				if ( Target_File )
				{
					fib = malloc( sizeof( struct FileInfoBlock ) );
					if ( fib )
					{
						Examine( lock, fib );
						if ( ( fib -> fib_Size ) > 0 )
						{
							AvailMem_LONG = ( AvailMem( MEMF_ANY|MEMF_LARGEST ) - 204800 );
							if ( AvailMem_LONG < 32768 ) AvailMem_LONG = 32768;

							if ( AvailMem_LONG > cfg_CopyBuf ) AvailMem_LONG = cfg_CopyBuf;
							if ( cfg_CopyVerify )
								if ( ( 2 * ( fib -> fib_Size ) ) > AvailMem_LONG ) MemSize_LONG = ( AvailMem_LONG / 2 ) + 1;
								else MemSize_LONG = ( fib -> fib_Size );
							else
								if ( ( fib -> fib_Size ) > AvailMem_LONG ) MemSize_LONG = AvailMem_LONG;
								else MemSize_LONG = ( fib -> fib_Size );

							Buffer[0] = malloc( MemSize_LONG );
							if ( Buffer[0] )
							{
								if ( cfg_CopyVerify ) Buffer[1] = malloc( MemSize_LONG );
								if ( Buffer[1] || !cfg_CopyVerify )
								{
									for ( i = 0; i * MemSize_LONG < ( fib -> fib_Size ); i++ )
									{
										DoMethod( app_RumorOpus, MUIM_Application_InputBuffered );
										get( wi_Progress, MUIA_Window_Open, &Opened );
										get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
										if ( Opened || Iconified || !Progress )
										{
											size_src = fread( Buffer[0], 1, MemSize_LONG, Source_File );
											size_dst = fwrite( Buffer[0], 1, size_src, Target_File );

											if( size_src != size_dst )
											{
												ErrorNum = IoErr();
												break;
											}

											if( cfg_CopyVerify )
											{
												fseek( Target_File, -size_dst, SEEK_END );
												size_src = fread( Buffer[1], 1, size_dst, Target_File );
												if( memcmp( Buffer[0], Buffer[1], size_src ) != 0 )
												{
													ErrorNum = -13;
													break;
												}
												fseek( Target_File, 0, SEEK_END );
											}

											if ( cfg_CopyStat && Progress )
											{
												pos = pos + size_dst;
												if ( ( ( fib -> fib_Size ) / 100 ) > 0 )
													Percent = ( pos / ( ( fib -> fib_Size ) / 100 ) );
												if ( Percent > 100 ) Percent = 100;
												sprintf( Status_String, Cat_String, Source_String, Target_String, NumberToString ( Percent ) );
												set( bt_StatusBar, MUIA_Text_Contents, Status_String );
											}
										}
										else
											ErrorNum = -12;
									}
									if ( cfg_CopyVerify ) free( Buffer[1] );
								}
								else
									ErrorNum = -1;
								free( Buffer[0] );
							}
							else
								ErrorNum = -1;
							free( fib );
						}
					}
					else
						ErrorNum = -1;
					fclose( Target_File );
				}
				else
					ErrorNum = IoErr();
				fclose( Source_File );
			}
			else
				ErrorNum = IoErr();

			if ( ErrorNum == 0 )
				Clone( Source_String, Target_String );
			else
				Delete( Target_String );
		}

		if ( Proceed == -1 )
			ErrorNum = -7;

		UnLock(lock);
	}
	else
		ErrorNum = IoErr();

	return( ErrorNum );
}

/*
**
** CopyDirectory()
**
*/

int CopyDirectory ( char * Source_String, char * Target_String )
{
	int ErrorNum = 0;
	__aligned struct FileInfoBlock * fib;
	char Source[256], Target[256];
	BPTR lock, dirlock;

	dirlock = CreateDir( Target_String );
	if ( dirlock )
		UnLock( dirlock );
	fib = malloc( sizeof( struct FileInfoBlock ) );
	if ( fib )
	{
		lock = Lock( Source_String, ACCESS_READ );
		if ( lock )
		{
			if ( Examine( lock, fib ) )
			{
				while( ExNext( lock, fib ) )
				{
					strcpy( Source, Source_String );
					strcpy( Target, Target_String );
					AddPart( Source, fib -> fib_FileName, sizeof( Source ) );
					AddPart( Target, fib -> fib_FileName, sizeof( Target ) );

					if ( fib -> fib_DirEntryType > 0 )
					{
						ErrorNum = CopyDirectory( Source, Target );

						if ( ErrorNum == 0 )
							Clone( Source, Target );
					}
					else
					{
						ErrorNum = CopyFile( Source, Target, TRUE );
						if ( ErrorNum == -20 )
							ErrorNum = 0;
					}
					if ( ErrorNum != 0 ) break;
				}
			}
			else
				ErrorNum = IoErr();
			UnLock( lock );
		}
		else
			ErrorNum = IoErr();
		free( fib );
	}
	else
		ErrorNum = -1;

	return( ErrorNum );
}

/*
**
** DeleteDirectory()
**
*/

int DeleteDirectory ( char * Path_String )
{
	int ErrorNum = 0;
	BPTR lock;
	__aligned struct FileInfoBlock * fib;
	BOOL IsDir_BOOL, FreeAll_BOOL = FALSE;
	char Source[256];

	strcpy( Source, Path_String );
	lock = Lock( Source, ACCESS_READ );
	if ( lock )
	{
		fib = malloc( sizeof( struct FileInfoBlock ) );
		if ( fib )
		{
			if ( Examine( lock, fib ) )
			{
				if ( ExNext( lock, fib ) )
				{
					AddPart( Source, fib -> fib_FileName, sizeof( Source ) );
					if ( fib -> fib_DirEntryType > 0 ) IsDir_BOOL = TRUE;
					if ( fib -> fib_DirEntryType < 0 ) IsDir_BOOL = FALSE;
					free( fib );
					UnLock( lock );
					FreeAll_BOOL = TRUE;

					if ( IsDir_BOOL ) ErrorNum = DeleteDirectory( Source );
					if ( ErrorNum == 0 )
					{
						if ( cfg_DelStat )
						{
							sprintf( Status_String, GetCatStr( 53, "Deleting '%s'..." ), Source );
							set( bt_StatusBar, MUIA_Text_Contents, Status_String );
						}
						ErrorNum = RemoveAFile( Source );
						if ( ErrorNum == 0 )
							ErrorNum = -10;
					}
				}
			}
			else
				ErrorNum = IoErr();
			if ( !FreeAll_BOOL )
				free( fib );
		}
		else
			ErrorNum = -1;
		if ( !FreeAll_BOOL )
			UnLock( lock );
	}
	else
		ErrorNum = IoErr();

	return( ErrorNum );
}

/*
**
** Delete()
**
*/

int Delete ( char * FileName_String )
{
	int ErrorNum = 0;
	BPTR lock;
	__aligned struct FileInfoBlock * fib;
	BOOL IsDir_BOOL;

	lock = Lock( FileName_String, ACCESS_READ );
	if ( lock )
	{
		fib = malloc( sizeof( struct FileInfoBlock ) );
		if ( fib )
		{
			Examine( lock, fib );
			if ( fib -> fib_DirEntryType > 0 ) IsDir_BOOL = TRUE;
			if ( fib -> fib_DirEntryType < 0 ) IsDir_BOOL = FALSE;
			free( fib );
		}
		else
			ErrorNum = -1;
		UnLock( lock );
	}
	else
		ErrorNum = IoErr();

	if ( ErrorNum == 0 )
	{
		if ( IsDir_BOOL )
		{
			do
			{
				ErrorNum = DeleteDirectory( FileName_String );
			}
			while ( ErrorNum == -10 );

			if ( ErrorNum == 0 )
				ErrorNum = RemoveAFile( FileName_String );
		}
		else
			ErrorNum = RemoveAFile( FileName_String );
	}

	return( ErrorNum );
}

/*
**
** Copy()
**
*/

int Copy ( char * FileName_String, char * Target_String )
{
	int ErrorNum = 0;
	BPTR lock;
	__aligned struct FileInfoBlock * fib;
	char Help_String_A[256], Help_String_B[256];
	BOOL IsDir_BOOL;

	lock = Lock( FileName_String, ACCESS_READ );
	if ( lock )
	{
		fib = malloc( sizeof( struct FileInfoBlock ) );
		if ( fib )
		{
			Examine( lock, fib );
			if ( fib -> fib_DirEntryType > 0 ) IsDir_BOOL = TRUE;
			if ( fib -> fib_DirEntryType < 0 ) IsDir_BOOL = FALSE;
			free( fib );
		}
		else
			ErrorNum = -1;
		UnLock( lock );
	}
	else
		ErrorNum = IoErr();

	if ( ErrorNum == 0 )
	{
		if ( !IsDir_BOOL )
			ErrorNum = CopyFile( FileName_String, Target_String, TRUE );
		else
		{
			sprintf( Help_String_A, "/%s/", FileName_String );
			sprintf( Help_String_B, "/%s/", Target_String );
			if ( ( strstr( Help_String_B, Help_String_A ) == NULL ) || ( stricmp( FileName_String, Target_String ) == 0 ) )
				ErrorNum = CopyDirectory( FileName_String, Target_String );
			else
				ErrorNum = -5;

			if ( ErrorNum == 0 )
				Clone( FileName_String, Target_String );
		}
	}

	return( ErrorNum );
}

/*
**
** BytesDirectory()
**
*/

int BytesDirectory ( char * Path_String, ULONG * Total_ULONG, ULONG * Files_ULONG, ULONG * Dirs_ULONG )
{
	BPTR lock;
	__aligned struct FileInfoBlock * fib;
	int ErrorNum = 0;
	char Path[256];
	LONG Opened, Iconified;

	strcpy( Path, Path_String );
	fib = malloc( sizeof( struct FileInfoBlock ) );
	if ( fib )
	{
		lock = Lock( Path_String, ACCESS_READ);
		if ( lock )
		{
			if ( Examine( lock, fib ) )
			{
				while ( ExNext ( lock, fib ) )
				{
					if ( fib -> fib_DirEntryType > 0 )
					{
						AddPart( Path, fib -> fib_FileName, sizeof( Path ) );
						*Dirs_ULONG = *Dirs_ULONG + 1;
						ErrorNum = BytesDirectory( Path, Total_ULONG, Files_ULONG, Dirs_ULONG);
						if ( ErrorNum !=0 ) break;
					}
					if ( fib -> fib_DirEntryType < 0 )
					{
						DoMethod( app_RumorOpus, MUIM_Application_InputBuffered );
						get( wi_Progress, MUIA_Window_Open, &Opened );
						get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
						if ( Opened || Iconified )
						{
							*Files_ULONG = *Files_ULONG + 1;
							*Total_ULONG = *Total_ULONG + fib -> fib_Size;
						}
						else
							ErrorNum = -12;
					}
					strcpy( Path, Path_String );
				}
			}
			else
				ErrorNum = IoErr();
			UnLock( lock );
		}
		else
			ErrorNum = IoErr();
		free( fib );
	}
	else
		ErrorNum = -1;
	if( IoErr() != ERROR_NO_MORE_ENTRIES ) ErrorNum = IoErr();

	return( ErrorNum );
}

/*
**
** FitDirectory()
**
*/

int FitDirectory ( char * Path_String, ULONG * Total_ULONG, ULONG * Files_ULONG, ULONG * Dirs_ULONG, ULONG BlockSize_ULONG )
{
	BPTR lock;
	__aligned struct FileInfoBlock * fib;
	int ErrorNum = 0;
	char Path[256];
	LONG Opened, Iconified;

	strcpy( Path, Path_String );
	fib = malloc( sizeof( struct FileInfoBlock ) );
	if ( fib )
	{
		lock = Lock( Path_String, ACCESS_READ );
		if ( lock )
		{
			if ( Examine( lock, fib ) )
			{
				while ( ExNext ( lock, fib ) )
				{
					if ( fib -> fib_DirEntryType > 0 )
					{
						AddPart( Path, fib -> fib_FileName, sizeof( Path ) );
						*Dirs_ULONG = *Dirs_ULONG + 1;
						ErrorNum = FitDirectory( Path, Total_ULONG, Files_ULONG, Dirs_ULONG, BlockSize_ULONG );
						if ( ErrorNum != 0) break;
					}
					if ( fib -> fib_DirEntryType < 0 )
					{
						DoMethod( app_RumorOpus, MUIM_Application_InputBuffered );
						get( wi_Progress, MUIA_Window_Open, &Opened );
						get( app_RumorOpus, MUIA_Application_Iconified, &Iconified );
						if ( Opened || Iconified )
						{
							*Files_ULONG = *Files_ULONG + 1;
							if ( ( fib -> fib_Size % BlockSize_ULONG ) > 0 )
								*Total_ULONG = *Total_ULONG + ( ( fib -> fib_Size / BlockSize_ULONG ) + 1 );
							else
								*Total_ULONG = *Total_ULONG + ( fib -> fib_Size / BlockSize_ULONG );
						}
						else
							ErrorNum = -12;
					}
					strcpy( Path, Path_String );
				}
			}
			else
				ErrorNum = IoErr();
			UnLock( lock );
		}
		else
			ErrorNum = IoErr();
		free( fib );
	}
	else
		ErrorNum = -1;
	if ( IoErr() != ERROR_NO_MORE_ENTRIES ) ErrorNum = IoErr();

	return( ErrorNum );
}

/*
**
** ExecuteCommand()
**
*/

int ExecuteCommand ( char * Command_String, char * File_String )
{
	int rc = 10;
	char Out_String[256], Exec_String[512];
	BPTR file, path, newlock, oldlock;
	BOOL Wait_BOOL, CD_Source, CD_Dest, NoBreak_BOOL = FALSE, Error = FALSE;

	if ( strlen( Command_String ) > 0 )
		strcpy( Exec_String, ParseCommand( Command_String, File_String, &Wait_BOOL, &CD_Source, &CD_Dest ) );

	strcpy( Out_String, cfg_Output );
	if ( Wait_BOOL )
		strcat( Out_String, "/WAIT" );
	file = Open( Out_String, MODE_OLDFILE );
	if ( !file )
	{
		strcpy( Out_String, "CON:0/0/640/200/Output/CLOSE/AUTO" );
		if ( Wait_BOOL )
			strcat( Out_String, "/WAIT" );
		file = Open( Out_String, MODE_OLDFILE );
	}

	if ( file )
	{
		if ( CD_Source || CD_Dest )
		{
			if ( CD_Source && global_DirLoaded[Active_Side])
			{
				newlock = Lock( GetPath( Active_Side ), ACCESS_READ );
				if ( newlock )
					oldlock = CurrentDir( newlock );
				else
					Error = TRUE;
			}

			if ( CD_Dest && global_DirLoaded[OtherSide(Active_Side)])
			{
				newlock = Lock( GetPath( OtherSide( Active_Side ) ), ACCESS_READ );
				if ( newlock )
					oldlock = CurrentDir( newlock );
				else
					Error = TRUE;
			}
		}

		if ( strlen( Exec_String ) > 0 && !Error )
		{
			if ( global_ARGC == 0 )
			{
				path = CloneWorkbenchPath((struct WBStartup *)global_ARGV);
				rc = SystemTags( Exec_String, SYS_Output, NULL, SYS_Input, file, NP_Path, path, NP_StackSize, 16384, TAG_DONE );
				if ( rc == -1 )
					FreeWorkbenchPath( path );
			}
			else
				rc = SystemTags( Exec_String, SYS_Output, NULL, SYS_Input, file, NP_StackSize, 16384, TAG_DONE );

			Delay(10);
		}
		else
			rc = 10;

		if ( ( CD_Source || CD_Dest ) && ( newlock ) )
		{
			UnLock( newlock );
			CurrentDir( oldlock );
		}

		Close( file );
	}

	if ( NoBreak_BOOL )
		rc = 0;

	return( rc );
}

/*
**
** ExecuteCommandNoOutput()
**
*/

int ExecuteCommandNoOutput ( char * Command_String, char * File_String )
{
	int rc = 10;
	char Exec_String[512];
	BPTR file, path, newlock, oldlock;
	BOOL Wait_BOOL, CD_Source, CD_Dest, NoBreak_BOOL = FALSE, Error = FALSE;

	if ( strlen( Command_String ) > 0 )
		strcpy( Exec_String, ParseCommand( Command_String, File_String, &Wait_BOOL, &CD_Source, &CD_Dest ) );

	file = Open( "NIL:", MODE_OLDFILE );
	if ( file )
	{
		if ( CD_Source || CD_Dest )
		{
			if ( CD_Source && global_DirLoaded[Active_Side])
			{
				newlock = Lock( GetPath( Active_Side ), ACCESS_READ );
				if ( newlock )
					oldlock = CurrentDir( newlock );
				else
					Error = TRUE;
			}

			if ( CD_Dest && global_DirLoaded[OtherSide(Active_Side)])
			{
				newlock = Lock( GetPath( OtherSide( Active_Side ) ), ACCESS_READ );
				if ( newlock )
					oldlock = CurrentDir( newlock );
				else
					Error = TRUE;
			}
		}

		if ( strlen( Exec_String ) > 0 && !Error )
		{
			if ( global_ARGC == 0 )
			{
				path = CloneWorkbenchPath((struct WBStartup *)global_ARGV);
				rc = SystemTags( Exec_String, SYS_Output, file, SYS_Input, file, NP_Path, path, NP_StackSize, 16384, TAG_DONE );
				if ( rc == -1 )
					FreeWorkbenchPath( path );
			}
			else
				rc = SystemTags( Exec_String, SYS_Output, file, SYS_Input, file, NP_StackSize, 16384, TAG_DONE );

			Delay(10);
		}
		else
			rc = 10;

		if ( ( CD_Source || CD_Dest ) && ( newlock ) )
		{
			UnLock( newlock );
			CurrentDir( oldlock );
		}

		Close( file );
	}

	if ( NoBreak_BOOL )
		rc = 0;

	return( rc );
}

/*
**
** ListArchive()
**
*/

int ListArchive ( char * Command_String, char * File_String )
{
	BPTR outfile, infile, path;
	char out[128], Exec_String[512];
	int rc = 10;
	BOOL Wait_BOOL, CD_Source, CD_Dest, NoBreak_BOOL = FALSE;

	strcpy( out, cfg_TempDir );
	infile=Open("NIL:",MODE_OLDFILE);
	if ( infile )
	{
		AddPart( out, "__TMP__", sizeof( out ) );
		outfile = Open( out, MODE_NEWFILE );

		if ( !outfile )
			outfile = Open( "T:__TMP__", MODE_NEWFILE );

		if ( outfile )
		{
			strcpy( Exec_String, ParseCommand( Command_String, File_String, &Wait_BOOL, &CD_Source, &CD_Dest ) );

			if ( global_ARGC == 0 )
			{
				path = CloneWorkbenchPath((struct WBStartup *)global_ARGV);
				rc = SystemTags( Exec_String, SYS_Output, outfile, SYS_Input, infile, NP_Path, path, NP_StackSize, 16384, TAG_DONE );
				if ( rc == -1 )
					FreeWorkbenchPath( path );
			}
			else
				rc = SystemTags( Exec_String, SYS_Output, outfile, SYS_Input, infile, NP_StackSize, 16384, TAG_DONE );

			Close(outfile);
			if ( rc == 0 )
			{
				rc = ExecuteCommand( cfg_FileType[1], out );
				Delay( cfg_DelDelay );
				DeleteFile( out );
			}
		}
		Close(infile);
	}

	if ( NoBreak_BOOL )
		rc = 0;

	return( rc );
}

/*
**
** FileAction()
**
*/

int FileAction ( char * File_String )
{
	BPTR lock;
	int ErrorNum = 0, rc = 0, Num;
	char string[256], newname[256];
	BOOL success;
	__aligned struct FileInfoBlock * fib;

	fib = malloc( sizeof( struct FileInfoBlock ) );
	if ( fib )
	{
		lock = Lock( File_String, ACCESS_READ );
		if ( lock )
		{
			Examine( lock, fib );
			if ( fib -> fib_Size > 0 )
			{
				SleepWindow(TRUE);
				Num = Recog( File_String );

				if ( Num == -4 )
				{
					if(XpkBase=OpenLibrary(XPKNAME,0))
					{
						strcpy(newname,cfg_TempDir);
						strcpy(string,FilePart(File_String));
						AddPart(newname,string,sizeof(newname));
						rc=XpkUnpackTags( XPK_InName, File_String, XPK_OutName, newname, XPK_NoClobber, TRUE, TAG_DONE	);
						if(rc==XPKERR_OK)
						{
							FileAction(newname);
							Delay(cfg_DelDelay);
							success=DeleteFile(newname);
							if(!success) ErrorNum = IoErr();
						}
						else ErrorNum = -9;
						CloseLibrary(XpkBase);
					}
					else ErrorNum = -8;
				}

				if ( Num == -3 )
				{
					sprintf( string, GetCatStr( 140, "File %s is an executable.\nDo you really want to start it?" ), File_String );
					if( (MUI_Request( app_RumorOpus, wi_Main, 0, GetCatStr( 50, "Request" ), GetCatStr( 139, "*_Okay|_Cancel" ), string, TAG_END ) ) == 1)
						rc = ExecuteCommand( "Run >NIL: %f &c", File_String );
				}

				if ( Num == -2 )
					rc = ExecuteCommand( cfg_FileType[1], File_String );

				if ( Num == -1 )
					Num = RecogArchive( File_String );

				if ( Num == -1 )
				{
					rc = ExecuteCommandNoOutput( cfg_FileType[2], File_String );
					if( rc != 0 )
						rc = ExecuteCommand( cfg_FileType[0], File_String );
						if( rc != 0 )
							ErrorNum = -4;
				}

				if ( Num >= 0 )
				{
					if( strlen( cfg_RecogCommandB[Num] ) > 0 )
						rc = ListArchive( cfg_RecogCommand[Num], File_String );
					else
						rc = ExecuteCommand( cfg_RecogCommand[Num], File_String );
				}

				SleepWindow( FALSE );
			}
			else
				ErrorNum = -4;

			UnLock( lock );
		}
		else
			ErrorNum = IoErr();
	}
	else
		ErrorNum = -1;

	if ( ErrorNum == 0 )
		if ( rc != 0 )
			ErrorNum = -4;

	return( ErrorNum );
}
