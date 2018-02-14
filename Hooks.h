/*
**
** Hooks.h
**
*/

extern APTR app_RumorOpus;
extern APTR pg_Page[2];
extern APTR bt_Button[120];
extern APTR bt_Drive[32];
extern APTR bt_StatusBar;
extern APTR wi_Main;
extern APTR wi_Progress;
extern APTR lv_Directory[2];
extern APTR ga_Gauge;

extern BOOL cfg_DropIcons;
extern BOOL cfg_DriveButtons;
extern BOOL global_DirLoaded[2];
extern BOOL cfg_ButtonType[171];
extern BOOL global_AskOnce;

extern LONG Active_Side;
extern LONG cfg_Overwrite;
extern LONG cfg_DelForce;

extern char cfg_ButtonCommand[171][81];
__far extern char cfg_Drive[32][81];
extern char Status_String[512];

extern void Reload(int side);
extern void Sleep(BOOL i);
extern void SleepClock(BOOL i);
extern void SleepWindow(BOOL i);
extern void Button ( int Num );
extern void DriveButton ( int Num );
extern void ActivateList ( int side );

extern BOOL CheckInfiniteLoop( char *from, char *to );
extern BOOL Clone( char *from, char *to);

extern int CopyFile( char *from, char *to, BOOL NoBreak );
extern int CopyDirectory ( char *from, char *to );
extern int LoadDirectory( char * buf, int side );
extern int FileAction(char *buf);
extern int OtherSide ( int side );
extern int StringSide ( char * Side_String );
extern int ExecuteCommand ( char * Command_String, char * File_String );
extern void UpdateNumFiles( int side );

extern char * Error( int error_number );
extern char * GetPath ( int side );
extern char * GetCatStr( int num, char *defstring );
extern char * StringRequester ( char *Title_String, char *Gadget_String, char *Reject_String, int MaxLength, BOOL SkipGadget, BOOL *Skip, BOOL *Cancel );
