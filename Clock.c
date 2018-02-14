#include "Includes.h"
#include "Clock.h"

/*
**
** ConvertNumberClock()
**
*/

char * ConvertNumberClock( ULONG num )
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
				Target[j] = '.';
			}
		}
		sprintf( Source, "%s", Target );
	}
	return( strdup ( Source ) );
}

/*
**
** ClockString()
**
*/

char * ClockString( void )
{
	char Clock_String[128], NewClock_String[128], Memory_String[81], Day_String[16], Date_String[16];
	char Time_String[9], NewTime_String[6], Avail_String[3][21];
	LONG AvailChip_LONG, AvailFast_LONG;
	struct DateStamp * datestamp;
	struct DateTime * datetime;

	strcpy( Clock_String, "" );

	datestamp = malloc( sizeof ( struct DateStamp ) );
	if ( datestamp )
	{
		DateStamp( datestamp );
		datetime = malloc( sizeof( struct DateTime ) );
		if ( datetime )
		{
			datetime -> dat_Stamp   = *datestamp;
			datetime -> dat_Format  = FORMAT_DOS;
			datetime -> dat_Flags   = NULL;
			datetime -> dat_StrDay  = Day_String;
			datetime -> dat_StrDate = Date_String;
			datetime -> dat_StrTime = Time_String;
			DateToStr( datetime );

			if ( cfg_ShowDay )
			{
				strcat( Clock_String, Day_String );
				strcat( Clock_String, " " );
			}

			if ( cfg_ShowDate )
			{
				strcat( Clock_String, Date_String );
				strcat( Clock_String, " " );
			}

			if ( cfg_ShowClock )
			{
				if ( !cfg_ShowSeconds )
					strmid( Time_String, NewTime_String, 1, 5 );
				else
					strcpy( NewTime_String, Time_String );

				strcat( Clock_String, NewTime_String );
				strcat( Clock_String, " " );
					
			}

			if ( ( cfg_ShowDay || cfg_ShowDate || cfg_ShowClock ) && (cfg_ShowMem) )
			{
				strcat( Clock_String, "- " );
			}

			if ( cfg_ShowMem )
			{
				AvailChip_LONG = AvailMem( MEMF_CHIP );
				AvailFast_LONG = AvailMem( MEMF_FAST );
				
				strcpy( Avail_String[0], ConvertNumberClock( AvailChip_LONG ) );
				strcpy( Avail_String[1], ConvertNumberClock( AvailFast_LONG ) );
				strcpy( Avail_String[2], ConvertNumberClock( AvailChip_LONG + AvailFast_LONG ) );

				sprintf( Memory_String, global_ClockFormat, Avail_String[0], Avail_String[1], Avail_String[2] );
				strcat ( Clock_String, Memory_String );
			}

			free( datetime );
		}
		free( datestamp );
	}

	strcpy( NewClock_String, Clock_String );
	return( NewClock_String );
}

/*
**
** ClockTask()
**
*/

LONG ClockTask ( void )
{
	int i;
	char String[128];

	while ( !global_QuitClock )
	{
		global_UpdateAllowed = FALSE;

		if ( ( ( global_SleepClock == 0 ) && !global_QuitClock ) && ( cfg_ShowClock || cfg_ShowDate || cfg_ShowDay || cfg_ShowMem ) )
		{
			strcpy( String, ClockString() );
			DoMethod( app_RumorOpus, MUIM_Application_PushMethod, bt_StatusBar, 3, MUIM_Set, MUIA_Text_Contents, String );
		}

		global_UpdateAllowed = TRUE;

		for ( i = 0 ; i < ( cfg_ClockRefresh / 50 ) ; i++ )
			if ( !global_QuitClock ) Delay( 50 );

		if ( !global_QuitClock ) Delay( cfg_ClockRefresh % 50 );
	}

	return( 0 );
}

/*
**
** InstallClock()
**
*/

void InstallClock ( void )
{
	global_ClockMessage = start_process( ClockTask, cfg_ClockPri, 4096 );

	if (!( cfg_ShowClock || cfg_ShowDate || cfg_ShowDay || cfg_ShowMem ))
		set( bt_StatusBar, MUIA_Text_Contents, global_IdleStatus );
}

/*
**
** RemoveClock()
**
*/

void RemoveClock ( void )
{
	if ( global_ClockMessage )
	{
		SetTaskPri( ( FindTask( "RO Clock" ) ), 10 );
		global_QuitClock = TRUE;
		wait_process( global_ClockMessage );
		DoMethod( app_RumorOpus, MUIM_Application_InputBuffered );
	}
}

/*
**
** UpdateClock()
**
*/

void UpdateClock( void )
{
	char String[128];

	if ( cfg_ShowClock || cfg_ShowDate || cfg_ShowDay || cfg_ShowMem )
	{
		if ( global_UpdateAllowed )
		{
			strcpy( String, ClockString() );
			set( bt_StatusBar, MUIA_Text_Contents, String );
		}
	}
	else
		set( bt_StatusBar, MUIA_Text_Contents, global_IdleStatus );
}
