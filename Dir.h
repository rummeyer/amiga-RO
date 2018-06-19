/*
**
** Dir.h
**
*/

extern APTR wi_Main;
extern APTR bt_StatusText[2];
extern APTR st_PathGadget[2];
extern APTR lv_Directory[2];
extern APTR lv_Buffers[2];
extern APTR lv_Volumes[2];
extern APTR pg_Page[2];

extern char global_Path[2][512];
extern char global_Userserial[5];

extern LONG cfg_History;
extern LONG global_Actions;
extern LONG global_NumBuffers;

extern BOOL global_DirLoaded[2];
extern BOOL global_KeyFile;
extern BOOL cfg_PathExpand;
extern BOOL cfg_CheckDupes;

extern BOOL AboutRequester ( void );
extern int OtherSide ( int side );
extern void SleepClock ( BOOL Sleep_BOOL );
extern void Fail ( void );
extern void SleepWindow ( BOOL Sleep_BOOL );
extern char * DevicePart ( char * Path_String );
extern char * GetPath ( int side );
extern char * GetCatStr ( int num, char *defstring );
extern char * NumberToString ( ULONG num );

BOOL global_Reload = FALSE;

LONG global_CursorPos[2];

void Reload ( int side );
void LoadParent ( int side );
void LoadRoot ( int side );
void UpdateStatusText ( int side );
void LoadVolume ( int side );
void LoadBuffer ( int side );
int LoadDirectory ( char * Path_String, int side );
