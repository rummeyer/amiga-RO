#include "PrefsIncludes.h"
#include "PrefsRequest.h"

/*
**
** AboutRequester()
**
*/

BOOL AboutRequester ( void )
{
	char About_String[512];

	if ( !global_Special )
	{
		if ( !global_KeyFile )
		{
			strcpy( About_String, "" );
			strcat( About_String, "\33c\33uRO Prefs 0.86 (5.12.98)\n\33n" );
			strcat( About_String, "Copyright © 1994-1998 by Oliver Rummeyer\n\n" );
			strcat( About_String, "\33b- Unregistered Release -\33n\n\n" );
			strcat( About_String, "To register, send $20.- or DM 30.- to\n\n" );
			strcat( About_String, "Oliver Rummeyer\n" );
			strcat( About_String, "Gartenstraﬂe 41\n" );
			strcat( About_String, "D-88212 Ravensburg\n" );
			strcat( About_String, "Germany/Europe\n\n" );
			strcat( About_String, "e-mail: oliver.rummeyer@student.uni-ulm.de\n\n" );
			strcat( About_String, "This is a MUI-Application\n" );
			strcat( About_String, "MUI is copyrighted by Stefan Stuntz" );
		}
		else
		{
			strcpy( About_String, "" );
			strcat( About_String, "\33c\33uRO Prefs 0.86 (5.12.98)\n\33n" );
			strcat( About_String, "Copyright © 1994-1998 by Oliver Rummeyer\n\n" );
			strcat( About_String, "\33b- Registered Release -\33n\n\n" );
			strcat( About_String, "Registered to\n" );
			strcat( About_String, global_Username );
			strcat( About_String, "\n(SerialNr " );
			strcat( About_String, global_Userserial );
			strcat( About_String, ")\n\n");
			strcat( About_String, "This is a MUI-Application\n" );
			strcat( About_String, "MUI is copyrighted by Stefan Stuntz" );
		}
	}
	else
	{
		strcpy( About_String, "" );
		strcat( About_String, "\33c\33uRO Prefs 0.86 (5.12.98)\n\33n" );
		strcat( About_String, "Copyright © 1994-1998 by Oliver Rummeyer\n\n" );
		strcat( About_String, "\33b- Special Edition for amigaOS CD Vol.1 -\33n\n\n" );
		strcat( About_String, "This is a MUI-Application\n" );
		strcat( About_String, "MUI is copyrighted by Stefan Stuntz" );
	}

	MUI_Request( app_RumorOpusPrefs, wi_Main, 0, "About RO Prefs", "_Okay", About_String, TAG_END );

	return( TRUE );
}
