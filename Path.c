#include "Includes.h"
#include "Path.h"

/*
**
** GetPath()
**
*/

char * GetPath ( int side )
{
	char * Path_String;
	get( st_PathGadget[side], MUIA_String_Contents, &Path_String );
	return Path_String;
}

/*
**
** FindMatch()
**
*/

char * FindMatch ( char * Path_String )
{
	BPTR lock;
	BOOL found=FALSE;
	char FilePart_String[256], * Pattern_Token;
	char Pattern_String[256], PathPart_String[512], String[512];
	__aligned struct FileInfoBlock * fib;

	strcpy( String, Path_String );
	strmid( String, PathPart_String, 1, strlen( String ) - strlen( FilePart( String ) ) );
	sprintf( Pattern_String, "%s#?", FilePart( Path_String ) );

	Pattern_Token = malloc(256);
	if ( Pattern_Token )
	{
		ParsePatternNoCase( Pattern_String, Pattern_Token, 256 );
		fib = malloc ( sizeof ( struct FileInfoBlock ) );
		if ( fib )
		{
			lock = Lock( PathPart_String, ACCESS_READ );
			if ( lock )
			{
				if ( Examine( lock, fib ) );
				{
					while( ExNext( lock, fib ) )
					{
						if (fib->fib_DirEntryType > 0)
						{
							strcpy( FilePart_String, fib->fib_FileName );
							if ( MatchPatternNoCase( Pattern_Token, fib->fib_FileName ) )
							{
								found = TRUE;
								break;
							}
						}
					}
				}
				UnLock( lock );
			}
			free( fib );
		}
		free( Pattern_Token );
	}
	if ( found )
	{
		AddPart( PathPart_String, FilePart_String, sizeof( PathPart_String ) );
		return PathPart_String;
	}

	return NULL;
}
