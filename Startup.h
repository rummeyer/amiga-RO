/*
**
** Startup.h
**
*/

char global_GroupSeparator[2];
char global_Exec_String[256];
char global_Pattern_String[81];
char Status_String[1024];
char global_ClockFormat[81];
extern LONG global_CursorPos[2];
extern BOOL global_DirLoaded[2];
LONG global_SleepClock = 0;
LONG global_CurrentBank;
char global_IdleStatus[41];
extern char MenuStrings[25][31];
extern char ShortHelp[5][256];

struct DiskObject * DiskObject;
struct Catalog * global_Catalog;

void Init ( void );
void Fail ( void );

extern BOOL global_UpdateAllowed;
extern void DefaultConfig ( void );
extern BOOL LoadConfig ( BOOL FirstPass );
extern void Methods ( void );
extern BOOL StartApplication ( void );
extern BOOL StopApplication ( void );
extern void SetMenu ( void );
extern char * GetCatStr( int num, char *defstring );
extern void InstallClock ( void );
extern void RemoveClock ( void );

extern APTR app_RumorOpus;

LONG __stack = 32768;
