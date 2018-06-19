/*
**
** Config.h
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
LONG cfg_SortType[2];
BOOL cfg_SortHighLow[2];
LONG cfg_LeftPage;
LONG cfg_RightPage;
LONG cfg_MiddleMouse;
LONG cfg_MultiSelect;
LONG global_MaximumBank;
LONG global_HotKeyNum;
LONG global_NumBuffers = 0;

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
BOOL cfg_ButtonType[171];
BOOL cfg_FlushMemory;
BOOL cfg_CheckDupes;
BOOL cfg_PageType;

char cfg_LeftLoad[81];
char cfg_RightLoad[81];
char cfg_Output[81];
char cfg_TempDir[81];
char cfg_DriveText[32][81];
char cfg_ButtonText[120][81];
char cfg_ButtonCommand[171][81];
char cfg_MenuEntry[20][81];
char cfg_MenuEntryText[20][41];
char cfg_FileType[3][81];
__far char cfg_Drive[32][81];
__far char cfg_HotDir[10][81];

__far char cfg_RecogString[100][41];
__far char cfg_RecogHex[100][41];
__far char cfg_RecogName[100][41];
__far char cfg_RecogCommand[100][81];
__far char cfg_RecogCommandB[100][81];

char cfg_Format[2][256];
char cfg_HotKey[50][41];

extern APTR Menu;
extern APTR men_UserMenu;
extern APTR lv_Buffers[2];
extern APTR bt_Drive[32];
extern APTR bt_Button[120];
extern APTR Row[2];
extern APTR pg_Page[2];
extern APTR app_RumorOpus;
extern struct Library * IntuitionBase;
extern struct Library * DOSBase;
extern struct Library * MUIMasterBase;

void DefaultConfig ( void );
BOOL LoadConfig ( BOOL FirstPass );
