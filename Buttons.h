/*
**
** Buttons.h
**
*/

extern struct ProcMsg * global_ClockMessage;

extern LONG Active_Side;
extern LONG global_MaximumBank;
extern LONG global_CurrentBank;

extern APTR lv_Directory[2];
extern APTR app_RumorOpus;
extern APTR bt_StatusBar;
extern APTR ButtonPage;

extern char cfg_ButtonCommand[171][81];
__far extern char cfg_HotDir[10][81];
extern char cfg_MenuEntry[20][81];
extern char Status_String[512];

extern BOOL cfg_ButtonType[171];
extern BOOL cfg_HotDirs;

extern void UpdateClock ( void );
extern void Sleep ( BOOL Sleep_BOOL );
extern void SleepClock ( BOOL Sleep_BOOL );
extern void SleepWindow ( BOOL Sleep_BOOL );
extern void ListCopy ( int side );
extern void ListSwap ( void );
extern void LoadRoot ( int side );
extern void LoadParent ( int side );
extern void ActivateList ( int side );
extern void Reload ( int side );
extern void Select ( int side );
extern void Update ( int side );
extern void Icon ( int side );
extern void DiskInfo ( int side );
extern void Change ( int side );
extern void Expand ( int side );
extern void Shrink ( int side );
extern void UpdateNumFiles ( int side );

extern int MakeAssign ( int side );
extern int ExecuteButton ( char * Exec_String, char * File_String );
extern int CreateDirectory ( int side, BOOL LoadIt );
extern int RelabelDevice ( int side );
extern int DoFunction ( int Operation, int side, char * Argument );
extern int LoadDirectory ( char * Path_String, int side );

extern char * GetCatStr ( int num, char *defstring );
extern char * Bytes ( int side );
extern char * Fit ( int side, BOOL window );
extern char * Error ( int error_number );

extern BOOL CheckMemory ( int bytes );
extern BOOL AboutRequester ( void );
