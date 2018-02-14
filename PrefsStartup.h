/*
**
** PrefsStartup.h
**
*/

extern void Methods ( void );
extern BOOL StartApplication ( void );
extern BOOL StopApplication ( void );
extern BOOL global_Cancel;

struct DiskObject * DiskObject;
struct Catalog * global_Catalog;

extern APTR app_RumorOpusPrefs;

LONG __stack = 8192;
