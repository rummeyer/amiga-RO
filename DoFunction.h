/*
**
** DoFunction.h
**
*/

extern BOOL global_QuitProgram;
extern BOOL global_DirLoaded[2];
extern BOOL global_AskOnce;
extern BOOL cfg_DelStat;
extern BOOL cfg_CopyStat;
extern BOOL cfg_DelVerify;
extern BOOL cfg_CheckFit;
extern BOOL cfg_Scroll;
extern BOOL cfg_PathExpand;
extern BOOL cfg_CreateIcons;

extern LONG cfg_DelForce;
extern LONG cfg_Completed;
extern LONG cfg_Overwrite;

extern APTR app_RumorOpus;
extern APTR wi_Main;
extern APTR ga_Gauge;
extern APTR wi_Progress;
extern APTR Menu;
extern APTR lv_Directory[2];
extern APTR bt_StatusBar;

extern char cfg_Output[81];
extern char cfg_TempDir[81];
extern char Status_String[1024];

__far extern char cfg_RecogString[100][41];
__far extern char cfg_RecogHex[100][41];
__far extern char cfg_RecogName[100][41];
__far extern char cfg_RecogCommand[100][81];
__far extern char cfg_RecogCommandB[100][81];

extern int global_ARGC;
extern char **global_ARGV;

extern void Reload ( int side );
extern void FunctionCompleted ( void );
extern void Fail ( void );
extern void UpdateNumFiles( int side );
extern void ParseMisc( char * Argument, BOOL * NoBreak_BOOL, BOOL * NoDeselect_BOOL );
extern void ParseReload( char * Argument, BOOL * ReloadSrc_BOOL, BOOL * ReloadDst_BOOL );
extern void ParseNeedTarget( int side, char * Argument, BOOL * Request_Dst );

extern BOOL QuitRequester ( void );
extern BOOL Exists( char * FileName_String );
extern BOOL Clone ( char * Source_String, char * Target_String );

extern int ExecuteCommand( char * Command_String, char * File_String );
extern int ExecuteCommandNoOutput ( char * Command_String, char * File_String );
extern int DeleteDirectory ( char * FileName_String );
extern int Delete ( char * FileName_String );
extern int Copy ( char * FileName_String, char * DestName_String );
extern int CopyFile ( char * FileName_String, char * DestName_String, BOOL NoBreak );
extern int CopyDirectory ( char * FileName_String, char * DestName_String );
extern int Overwrite ( char * Source_String, char * File_String );
extern int OtherSide ( int side );
extern int FileAction ( char * FileName_String );
extern int ListArchive( char * Command_String, char * FileName_String );
extern int Recog ( char * FileName_String );
extern int RecogArchive ( char * FileName_String );
extern int RemoveFile ( char * File_String );
extern int Overwrite ( char * Source_String, char * File_String );
extern int LoadDirectory ( char * Path_String, int side );

extern char * GetPath ( int side );
extern char * Fit ( int side, BOOL window );
extern char * StringRequester ( char *Title_String, char *Gadget_String, char *Reject_String, int MaxLength, int SkipGadget, BOOL *Skip, BOOL *Cancel );
extern char * GetCatStr( int num, char *defstring );
extern char * NumberToString( ULONG num );
extern char * ParseCommand( char * String, char * File_Name );
extern char * GetVersion ( char * FileName_String );
