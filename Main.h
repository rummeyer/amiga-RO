/*
**
** Main.h
**
*/

extern BOOL global_KeyFile;
extern BOOL global_DirLoaded[2];
extern BOOL global_AskOnce;
extern BOOL cfg_CopyDate;
extern BOOL cfg_CopyFlags;
extern BOOL cfg_CopyNote;
extern BOOL cfg_CopyVerify;
extern LONG cfg_DelForce;
extern BOOL cfg_DelVerify;
extern BOOL cfg_DropIcons;
extern BOOL cfg_CheckFit;
extern LONG global_SleepClock;
extern BOOL global_Illegal;

extern LONG cfg_LeftPage;
extern LONG cfg_RightPage;
extern LONG Active_Side;
extern LONG cfg_Overwrite;
extern LONG cfg_MiddleMouse;

extern APTR wi_Main;
extern APTR app_RumorOpus;
extern APTR bt_StatusBar;
extern APTR pg_Page[2];
extern APTR st_PathGadget[2];
extern APTR lv_Directory[2];
extern APTR wi_Progress;
extern APTR lv_Buffers[2];
extern APTR lv_Volumes[2];
extern APTR bt_StatusText[2];
extern APTR bt_Button[24];
extern APTR bt_Drive[32];
extern APTR men_UserMenu;
extern APTR ga_Gauge;
extern APTR Row[5];
extern APTR pg_Page[2];
extern APTR Menu;
extern APTR ButtonPage;

extern struct DiskObject * DiskObject;

extern char cfg_LeftLoad[81];
extern char cfg_RightLoad[81];
extern char Status_String[512];

extern void Init ( void );
extern void CheckKey ( void );
extern void Fail ( void );
extern void LoadVolume ( int side );
extern void LoadBuffer ( int side );
extern void LoadParent ( int side );
extern void UpdateStatusText ( int side );
extern void SetActiveColor( int side );
extern void DriveButton ( int i );
extern void Activate ( int side );
extern void HotDir( int num );
extern void MenuCommand( int num );
extern void SleepClock ( BOOL Sleep );
extern void UpdateClock ( void );
extern void SleepWindow( BOOL Sleep_BOOL );
extern void BankSwitch( void );
extern void Button( int i );
extern void UpdateNumFiles( int side );
extern void Sleep ( BOOL Sleep_BOOL );
extern void ActivateList( int side );
extern void Reload( int side );

extern BOOL AboutRequester ( void );
extern BOOL QuitRequester ( void );
extern BOOL Exists( char * File_String );

extern int ExecuteCommandNoOutput ( char * Command_String, char * File_String );
extern int LoadDirectory ( char * Path_String, int side );
extern int FileAction ( char * File_String );
extern int DoFunction( int Operation, int side, char * Argument );
extern int OtherSide( int side );
extern int MouseSide ( int X, int Y );
extern char * GetPath ( int side );
extern char * FindMatch ( char * Path_String );
extern char * GetCatStr( int num, char *defstring );

BOOL global_QuitProgram = FALSE;

int global_ARGC;
char **global_ARGV;

char global_ARexxName[11];
char global_Path[2][512];

int CXBRK(void) { return(0); }
int _CXBRK(void) { return(0); }
void chkabort(void) {}

extern void DefaultConfig( void );
extern void Methods( void );
extern void StopApplication( void );
extern void SetMenu ( void );
extern BOOL LoadConfig( BOOL FirstPass );
extern BOOL StartApplication( void );
