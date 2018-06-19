extern BOOL cfg_CreateIcons;
extern BOOL cfg_PathExpand;
extern BOOL global_DirLoaded[2];
extern BOOL global_QuitProgram;

extern APTR lv_Directory[2];
extern APTR app_RumorOpus;
extern APTR ga_Gauge;
extern APTR wi_Progress;
extern APTR bt_StatusBar;
extern APTR bl_Balance;
extern APTR pg_Page[2];

extern LONG cfg_Completed;
extern LONG Active_Side;
extern LONG cfg_SortType[2];
extern LONG cfg_FirstType[2];
extern BOOL cfg_SortHighLow[2];

extern char global_Pattern_String[81];

extern BOOL QuitRequester ( void );
extern void Reload( int side );
extern void Fail ( void );

extern int ExecuteCommand ( char * Command_String, char * FileName_String, BOOL Wait );
extern int BytesDirectory ( char * Path_String, ULONG * Total_ULONG, ULONG * Files_ULONG, ULONG * Dirs_ULONG );
extern int FitDirectory ( char * Path_String, ULONG * Total_ULONG, ULONG * Files_ULONG, ULONG * Dirs_ULONG, ULONG BlockSize_ULONG );
extern int CopyFile ( char * Source_String, char * Target_String, BOOL NoBreak );
extern int LoadDirectory( char * Path_String, int side );
extern int OtherSide( int side );
extern void ActivateList( int side );

extern char * GetCatStr( int num, char *defstring );
extern char * NumberToString( ULONG num );
extern char * StringRequester ( char *Title_String, char *Gadget_String, char *Reject_String, int MaxLength, int SkipGadget, BOOL *Skip, BOOL *Cancel );
extern char * GetPath ( int side );
extern char * Error ( int error_number );

BOOL global_Unfolded = TRUE;

extern APTR FoldGroup;
