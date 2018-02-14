#include "PrefsIncludes.h"
#include "PrefsStartup.h"

/*
**
** Fail()
**
*/

void Fail ( void )
{
	if (DiskObject)
		FreeDiskObject( DiskObject );

	StopApplication();

	if ( global_Catalog )
		CloseCatalog( global_Catalog );

	if ( MUIMasterBase )
		CloseLibrary( MUIMasterBase );
	if ( global_Cancel )
		exit(5);
	else
		exit(0);
}

/*
**
** Init()
**
*/

void Init ( void )
{
	if (!(MUIMasterBase = OpenLibrary(MUIMASTER_NAME,MUIMASTER_VMIN)))
		Fail();

	if (!(StartApplication()))
		Fail();

	Methods();

	DiskObject = GetDiskObject( "PROGDIR:ROPrefs" );
	if ( DiskObject ) set(app_RumorOpusPrefs, MUIA_Application_DiskObject, DiskObject );
}
