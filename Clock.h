/*
**
** Clock.h
**
*/

extern APTR app_RumorOpus;
extern APTR bt_StatusBar;
extern LONG cfg_ClockRefresh;
extern LONG cfg_ClockPri;
extern BOOL cfg_ShowMem;
extern BOOL cfg_ShowClock;
extern BOOL cfg_ShowSeconds;
extern BOOL cfg_ShowDate;
extern BOOL cfg_ShowDay;
extern char global_ClockFormat[81];
extern char global_IdleStatus[41];
extern char global_GroupSeparator[2];

extern struct Library * DOSBase;
extern struct Library * IntuitionBase;
extern struct Catalog *global_Catalog;
extern struct ProcMsg *start_process (long (*)(void), long, long);
extern long wait_process (struct ProcMsg *);

struct ProcMsg * global_ClockMessage = NULL;
BOOL global_QuitClock = FALSE;
BOOL global_UpdateAllowed = TRUE;
extern LONG global_SleepClock;

void UpdateClock ( void );
void InstallClock ( void );
void RemoveClock ( void );
LONG ClockTask ( void );
char * ClockString ( void );
