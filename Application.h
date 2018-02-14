/*
**
** Application.h
**
*/

extern SAVEDS ASM LONG AppMsgFunc( REG(a2) APTR obj, REG(a1) struct AppMessage **x );
extern SAVEDS ASM LONG ButtonFunc( REG(a2) APTR obj );
extern SAVEDS ASM LONG DriveFunc( REG(a2) APTR obj );
extern SAVEDS ASM LONG ActionRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG ActivateRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG LoadRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG PathRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG RefreshRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG MessageRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG ChoiceRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG StringRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG NumEntriesRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG GetEntryRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG CommandRexxFunc( REG(a1) ULONG *arg );
extern SAVEDS ASM LONG FunctionRexxFunc( REG(a1) ULONG *arg );

extern char * GetCatStr( int num, char *defstring );

extern struct Catalog *global_Catalog;
extern LONG cfg_SortType[2];
extern LONG cfg_FirstType[2];
extern LONG cfg_ScrollerPos[2];
extern char cfg_MenuEntryText[20][41];
extern char cfg_ButtonText[120][81];
extern char cfg_LeftFormat[256];
extern char cfg_RightFormat[256];
extern char cfg_HotKey[50][41];

APTR app_RumorOpus;
APTR wi_Main;
APTR wi_Progress;
APTR lv_Directory[2];
APTR lv_Dir[2];
APTR lv_Buffers[2];
APTR lv_Volumes[2];
APTR st_PathGadget[2];
APTR bt_StatusText[2];
APTR bt_StatusBar;
APTR bt_Button[120];
APTR bt_Drive[32];
APTR pg_Page[2];
APTR Row[2];
APTR ga_Gauge;
APTR men_UserMenu;
APTR ButtonPage;
APTR Menu;

static char *Pages[] = {
"Directory",
"Buffers",
"Volumes",
NULL };

char MenuStrings[22][31];

struct NewMenu MenuData[] =
{
	{ NM_TITLE, MenuStrings[0] , 0 ,0,0,(APTR)0             },
	{ NM_ITEM , MenuStrings[21],"U",0,0,(APTR)ID_UpdateAll  },
	{ NM_ITEM , MenuStrings[20],"M",0,0,(APTR)ID_Message    },
	{ NM_ITEM , NM_BARLABEL    , 0 ,0,0,(APTR)0             },
	{ NM_ITEM , MenuStrings[1] ,"?",0,0,(APTR)ID_About      },
	{ NM_ITEM , NM_BARLABEL    , 0 ,0,0,(APTR)0             },
	{ NM_ITEM , MenuStrings[2] ,"I",0,0,(APTR)ID_Iconify    },
	{ NM_ITEM , MenuStrings[3] ,"Q",0,0,(APTR)MUIV_Application_ReturnID_Quit },
	{ NM_TITLE, MenuStrings[4] , 0 ,0,0,(APTR)0             },
	{ NM_ITEM,  MenuStrings[5] , 0 ,0,0,(APTR)0             },
	{ NM_SUB,   MenuStrings[6] , 0 ,CHECKIT|MENUTOGGLE,0,(APTR)ID_CopyFlags  },
	{ NM_SUB,   MenuStrings[7] , 0 ,CHECKIT|MENUTOGGLE,0,(APTR)ID_CopyDate   },
	{ NM_SUB,   MenuStrings[8] , 0 ,CHECKIT|MENUTOGGLE,0,(APTR)ID_CopyNote   },
	{ NM_SUB,   NM_BARLABEL    , 0 ,0,0,(APTR)0             },
	{ NM_SUB,   MenuStrings[19],"F",CHECKIT|MENUTOGGLE,0,(APTR)ID_CopyVerify },
	{ NM_SUB,   MenuStrings[9] ,"D",CHECKIT|MENUTOGGLE,0,(APTR)ID_CheckSpace },
	{ NM_ITEM , MenuStrings[10], 0 ,0,0,(APTR)0             },
	{ NM_SUB ,  MenuStrings[11], 0 ,CHECKIT|MENUTOGGLE,0,(APTR)ID_Verify     },
	{ NM_SUB ,  MenuStrings[12], 0 ,CHECKIT|MENUTOGGLE,0,(APTR)ID_Force      },
	{ NM_ITEM , MenuStrings[13], 0 ,0,0,(APTR)0             },
	{ NM_SUB ,  MenuStrings[14], 0 ,CHECKIT,2|4|8,(APTR)ID_OverYes    },
	{ NM_SUB ,  MenuStrings[15], 0 ,CHECKIT,1|4|8,(APTR)ID_OverNo     },
	{ NM_SUB ,  MenuStrings[16], 0 ,CHECKIT,1|2|8,(APTR)ID_OverOlder  },
	{ NM_SUB ,  MenuStrings[17], 0 ,CHECKIT,1|2|4,(APTR)ID_OverVerify },
	{ NM_TITLE, MenuStrings[18], 0 ,0,0,(APTR)0             },
	{ NM_ITEM,  "MUI...", 0 ,0,0,(APTR)ID_MUIPrefs },
	{ NM_ITEM,  "RO...", 0 ,0,0,(APTR)ID_ROPrefs },
	{ NM_END ,  NULL           , 0 ,0,0,(APTR)0             }
};

static const struct Hook ActionRexxHook = { { NULL,NULL },(VOID *)ActionRexxFunc,NULL,NULL };
static const struct Hook ActivateRexxHook = { { NULL,NULL },(VOID *)ActivateRexxFunc,NULL,NULL };
static const struct Hook LoadRexxHook = { { NULL,NULL },(VOID *)LoadRexxFunc,NULL,NULL };
static const struct Hook PathRexxHook = { { NULL,NULL },(VOID *)PathRexxFunc,NULL,NULL };
static const struct Hook RefreshRexxHook = { { NULL,NULL },(VOID *)RefreshRexxFunc,NULL,NULL };
static const struct Hook MessageRexxHook = { { NULL,NULL },(VOID *)MessageRexxFunc,NULL,NULL };
static const struct Hook ChoiceRexxHook = { { NULL,NULL },(VOID *)ChoiceRexxFunc,NULL,NULL };
static const struct Hook StringRexxHook = { { NULL,NULL },(VOID *)StringRexxFunc,NULL,NULL };
static const struct Hook NumEntriesRexxHook = { { NULL,NULL },(VOID *)NumEntriesRexxFunc,NULL,NULL };
static const struct Hook GetEntryRexxHook = { { NULL,NULL },(VOID *)GetEntryRexxFunc,NULL,NULL };
static const struct Hook CommandRexxHook = { { NULL,NULL },(VOID *)CommandRexxFunc,NULL,NULL };
static const struct Hook FunctionRexxHook = { { NULL,NULL },(VOID *)FunctionRexxFunc,NULL,NULL };

static struct MUI_Command ARexxCommands[] =
{
	{"active",		"SIDE/A",					1,				&ActivateRexxHook},
	{"choicereq",	"STRING/A",					1,				&ChoiceRexxHook},
	{"command",		"STRING/A,FILE/A",			2,				&CommandRexxHook},
	{"fileaction",  "FILE/A",					1,				&ActionRexxHook},
	{"function",  	"NAME/A",					1,				&FunctionRexxHook},
	{"getentry",	"SIDE/A,TYPE/A",			2,				&GetEntryRexxHook},
	{"load",		"SIDE/A,DIR/A",				2,				&LoadRexxHook},
	{"message",		"STRING/A",					1,				&MessageRexxHook},
	{"numentries",	"SIDE/A,TYPE/A",			2,				&NumEntriesRexxHook},
	{"path",		"SIDE/A",					1,				&PathRexxHook},
	{"refresh",		"SIDE/A",					1,				&RefreshRexxHook},
	{"sleep",		MC_TEMPLATE_ID,				ID_Sleep,		NULL},
	{"stringreq",	"STRING/A",					1,				&StringRexxHook},
	{"wakeup",		MC_TEMPLATE_ID,				ID_WakeUp,		NULL},

	{NULL,			NULL,				0,				NULL}
};

struct Library *MUIMasterBase;

void Methods ( void );
BOOL StartApplication ( void );
BOOL StopApplication ( void );

extern char Status_String[512];
extern LONG Active_Side;
extern int DoFunction ( int Operation, int side, char * Argument );
extern BOOL CheckMemory ( int bytes );
extern void Sleep ( BOOL Sleep_BOOL );
extern void SleepClock ( BOOL Sleep_BOOL );
extern void SleepWindow ( BOOL Sleep_BOOL );
extern char * Error ( int error_number );
extern BOOL global_DirLoaded[2];
extern int OtherSide( int side );
