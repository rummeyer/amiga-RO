extern APTR wi_Main;
extern APTR wi_Progress;
extern APTR app_RumorOpus;
extern APTR bt_StatusBar;

extern BOOL cfg_CopyDate;
extern BOOL cfg_CopyVerify;
extern BOOL cfg_CopyNote;
extern BOOL cfg_CopyFlags;
extern BOOL cfg_CopyStat;
extern BOOL cfg_DelStat;
extern BOOL cfg_ArcHandler;
extern BOOL global_DirLoaded[2];

extern char cfg_TempDir[81];
extern char cfg_Output[81];
extern char cfg_TempDir[81];

__far extern char cfg_RecogString[100][41];
__far extern char cfg_RecogHex[100][41];
__far extern char cfg_RecogName[100][41];
__far extern char cfg_RecogCommand[100][81];
__far extern char cfg_RecogCommandB[100][81];
extern char Status_String[1024];

extern char cfg_FileType[3][81];

extern LONG cfg_DelForce;
extern LONG cfg_DelDelay;
extern LONG cfg_CopyBuf;
extern LONG Active_Side;
extern LONG cfg_Overwrite;

extern int OtherSide( int side );
extern int Recog ( char * FileName_String );
extern int RecogArchive ( char * FileName_String );

extern int global_ARGC;
extern char **global_ARGV;

struct Library *XpkBase;

extern void SleepWindow ( BOOL Sleep_BOOL );
extern void Expand ( int side );
extern char * GetPath ( int side );
extern char * GetCatStr( int num, char *defstring );
extern char * ParseCommand( char * String, char * File_Name, BOOL * Wait_BOOL, BOOL * CD_Source, BOOL * CD_Dest );
extern int LoadDirectory ( char * Filename, int side );
extern char * NumberToString ( ULONG num );

BOOL Exists( char * File_String );
BOOL Clone ( char * Source_String, char * Target_String );

int Overwrite ( char * Source_String, char * File_String );
int CopyFile ( char * Source_String, char * Target_String, BOOL NoBreak );
int CopyDirectory ( char * Source_String, char * Target_String );
int Delete ( char * FileName_String );
int DeleteDirectory ( char * Path_String );
int BytesDirectory ( char * Path_String, ULONG * Total_ULONG, ULONG * Files_ULONG, ULONG * Dirs_ULONG );
int FitDirectory ( char * Path_String, ULONG * Total_ULONG, ULONG * Files_ULONG, ULONG * Dirs_ULONG, ULONG BlockSize_ULONG );
int ExecuteCommand ( char * Command_String, char * File_String );
int ListArchive ( char * Command_String, char * File_String );
int FileAction ( char * File_String );
