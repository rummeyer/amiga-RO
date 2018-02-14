/*
**
** PrefsConfig.h
**
*/

LONG cfg_ClockPri;
LONG cfg_ClockRefresh;
LONG cfg_Completed;
LONG cfg_CopyBuf;
LONG cfg_DelDelay;
LONG cfg_FirstType[2];
LONG cfg_History;
LONG cfg_Overwrite;
LONG cfg_ScrollerPos[2];
LONG cfg_SortType[2];
LONG cfg_LeftPage;
LONG cfg_RightPage;
LONG cfg_MiddleMouse;
LONG CurrentType;

BOOL cfg_CheckFit;
BOOL cfg_CopyDate;
BOOL cfg_CopyFlags;
BOOL cfg_CopyNote;
BOOL cfg_CopyStat;
BOOL cfg_CopyVerify;
BOOL cfg_CreateIcons;
LONG cfg_DelForce;
BOOL cfg_DelStat;
BOOL cfg_DelVerify;
BOOL cfg_DriveButtons;
BOOL cfg_DropIcons;
BOOL cfg_HotDirs;
BOOL cfg_PathExpand;
BOOL cfg_QuitVerify;
BOOL cfg_Scroll;
BOOL cfg_ShowClock;
BOOL cfg_ShowSeconds;
BOOL cfg_ShowDate;
BOOL cfg_ShowDay;
BOOL cfg_ShowMem;
BOOL cfg_FlushMemory;
BOOL cfg_CheckDupes;

BOOL LoadConfig ( char * filename );
void DefaultConfig( void );

extern APTR st_TempDir;
extern APTR st_Output;
extern APTR st_History;
extern APTR st_ClockPri;
extern APTR st_Refresh;
extern APTR st_Buffer;
extern APTR st_Delay;
extern APTR st_LoadLeft;
extern APTR st_FormatLeft;
extern APTR st_LoadRight;
extern APTR st_FormatRight;
extern APTR cy_AppWindow;
extern APTR cy_Completed;
extern APTR cy_DriveMode;
extern APTR cy_HotMode;
extern APTR cy_MiddleMouse;
extern APTR cy_Overwrite;
extern APTR cy_PageLeft;
extern APTR cy_FirstLeft;
extern APTR cy_SortLeft;
extern APTR cy_PageRight;
extern APTR cy_FirstRight;
extern APTR cy_SortRight;
extern APTR cm_CheckFit;
extern APTR cm_FlushMemory;
extern APTR cm_CopyVerify;
extern APTR cm_CopyDate;
extern APTR cm_CopyFlags;
extern APTR cm_CopyNote;
extern APTR cm_CreateIcons;
extern APTR cm_CopyStat;
extern APTR cm_DelStat;
extern APTR cm_DelVerify;
extern APTR cm_DelForce;
extern APTR cm_PathExpand;
extern APTR cm_QuitVerify;
extern APTR cm_FollowScroll;
extern APTR cm_ShowDay;
extern APTR cm_ShowDate;
extern APTR cm_ShowTime;
extern APTR cm_ShowSeconds;
extern APTR cm_ShowMemory;
extern APTR cm_CheckDupes;
extern APTR cm_Symmetrical;
extern APTR st_HotDir[10];
extern APTR lv_Input;
extern APTR lv_Filetypes;
extern APTR cy_Filetype;
extern APTR cy_Event;
extern APTR cy_Type;
extern APTR st_Command;
extern APTR st_CommandA;
extern APTR st_CommandB;
extern APTR st_Pattern;
extern APTR st_Recog;
extern APTR bt_FormatLeft;
extern APTR bt_FormatRight;
extern APTR pp_Command;
extern APTR pp_Function;
extern APTR pp_Directory;

BOOL cfg_Type[220];
char cfg_String[220][81];
int cfg_Event[220];

char cfg_FileType[3][81];

__far BOOL cfg_RecogType[100];
__far char cfg_RecogString[100][41];
__far char cfg_RecogHex[100][41];
__far char cfg_RecogCommand[100][81];
__far char cfg_RecogCommandB[100][81];

extern char *CYA_AppWindow[];
extern char *CYA_Completed[];
extern char *CYA_Mode[];
extern char *CYA_MiddleMouse[];
extern char *CYA_Overwrite[];
extern char *CYA_Pages[];
extern char *CYA_First[];
extern char *CYA_Sort[];
extern char *CYA_Event[];

char global_FileName[256];
