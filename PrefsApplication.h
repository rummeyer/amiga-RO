void Methods ( void );
BOOL StartApplication ( void );
BOOL StopApplication ( void );

APTR app_RumorOpusPrefs;
APTR wi_Main;

APTR Menu;
APTR bt_Save;
APTR bt_Use;
APTR bt_Cancel;
APTR pg_Page;
APTR lv_Filetypes;
APTR st_Filetypes;
APTR cy_AppWindow;
APTR cy_Completed;
APTR cy_DriveMode;
APTR cy_HotMode;
APTR cy_MiddleMouse;
APTR cy_Overwrite;
APTR st_TempDir;
APTR st_History;
APTR st_Output;
APTR st_Buffer;
APTR st_Delay;
APTR st_LoadLeft;
APTR st_FormatLeft;
APTR cy_PageLeft;
APTR cy_FirstLeft;
APTR cy_SortLeft;
APTR st_LoadRight;
APTR st_FormatRight;
APTR cy_PageRight;
APTR cy_FirstRight;
APTR cy_SortRight;
APTR st_Refresh;
APTR st_ClockPri;
APTR st_Recog;
APTR st_Pattern;
APTR st_CommandA;
APTR st_CommandB;
APTR cy_Filetype;
APTR lv_Input;
APTR st_Input;
APTR cy_Event;
APTR cy_Type;
APTR st_Command;
APTR st_Directory;
APTR st_HotDir[10];
APTR cm_CheckFit;
APTR cm_FlushMemory;
APTR cm_CopyVerify;
APTR cm_CopyDate;
APTR cm_CopyFlags;
APTR cm_CopyNote;
APTR cm_CreateIcons;
APTR cm_CopyStat;
APTR cm_DelStat;
APTR cm_DelVerify;
APTR cm_DelForce;
APTR cm_PathExpand;
APTR cm_QuitVerify;
APTR cm_FollowScroll;
APTR cm_ShowDay;
APTR cm_ShowDate;
APTR cm_ShowTime;
APTR cm_ShowSeconds;
APTR cm_ShowMemory;
APTR cm_CheckDupes;
APTR cm_Symmetrical;
APTR bt_AddEvent;
APTR bt_DelEvent;
APTR bt_AddFiletype;
APTR bt_DelFiletype;
APTR pp_Directory;
APTR pp_Command;
APTR pp_FunList;
APTR pp_ComList;
APTR pp_FormatLeft;
APTR pp_FormatRight;
APTR pp_FormatLeftList;
APTR pp_FormatRightList;
APTR bt_FormatLeft;
APTR bt_FormatRight;
APTR pp_Function;
APTR st_Function;
APTR pp_CommandA;
APTR pp_CommandB;
APTR pp_ComAList;
APTR pp_ComBList;

struct Library *MUIMasterBase;

static char *Pages[] = {
"Switches",
"Variables",
"Listviews",
"Hot Dirs",
"Filetypes",
"Input Events",
NULL };

const char *CYA_AppWindow[] = { "Load","Copy", NULL };
const char *CYA_Completed[] = { "Flash","Popup", "Quiet", NULL };
const char *CYA_Mode[] = { "Normal","Active", NULL };
const char *CYA_MiddleMouse[] = { "Both", "List","Bank", "None", NULL };
const char *CYA_Overwrite[] = { "Always", "Never", "Older", "Verify", NULL };
const char *CYA_Pages[] = { "Directory", "Buffers", "Volumes", NULL };
const char *CYA_First[] = { "Dirs", "Files", "Mixed", NULL };
const char *CYA_Sort[] = { "Name", "Date", "Size", NULL };
const char *CYA_Event[] = { "Hotkey", "Button", "Drive", "Menu", "Buffer", NULL };
const char *CYA_TypeBtn[] = { "Function", "Command", NULL };
const char *CYA_Type[] = { "Custom", "Archive", NULL };

struct NewMenu MenuData[] =
{
	{ NM_TITLE, "Project" ,   0 ,0,0,(APTR)0                      },
	{ NM_ITEM , "Open...",   "O",0,0,(APTR)ID_MenuOpen            },
	{ NM_ITEM , "Save As...","A",0,0,(APTR)ID_MenuSaveAs          },
	{ NM_ITEM , NM_BARLABEL,  0 ,0,0,(APTR)0                      },
	{ NM_ITEM , "About"     ,"?",0,0,(APTR)ID_MenuAbout           },
	{ NM_ITEM , NM_BARLABEL,  0 ,0,0,(APTR)0                      },
	{ NM_ITEM , "Iconify",   "I",0,0,(APTR)ID_MenuIconify         },
	{ NM_ITEM , "Quit",      "Q",0,0,(APTR)MUIV_Application_ReturnID_Quit },
	{ NM_TITLE, "Edit" ,      0 ,0,0,(APTR)0                      },
	{ NM_ITEM , "Reset To Defaults", "D",0,0,(APTR)ID_MenuReset   },
	{ NM_ITEM , "Last Saved",        "L",0,0,(APTR)ID_MenuLast    },
	{ NM_ITEM , "Restore",           "R",0,0,(APTR)ID_MenuRestore },
	{ NM_TITLE, "Settings" ,          0 ,0,0,(APTR)0              },
	{ NM_ITEM , "MUI...",             0 ,0,0,(APTR)ID_MUIPrefs    },
	{ NM_END ,  NULL                , 0 ,0,0,(APTR)0              }
};

static const char *PopVars[] =
{
	"%% - <percent>",
	"%a - <arexx portname>",
	"%F - <sourcepath>+<sourcefilename>",
	"%f - \"<sourcepath>+<sourcefilename>\"",
	"%N - <sourcefilename>",
	"%n - \"<sourcefilename>\"",
	"%P - <sourcepath>",
	"%p - \"<sourcepath>\"",
	"%T - <targetpath>",
	"%t - \"<targetpath>\"",
	"%S - <sourcedevice>",
	"%s - \"<sourcedevice>\"",
	"%D - <targetdevice>",
	"%d - \"<targetdevice>\"",
	"%R - <request string once>",
	"%r - <request string for each>",
	"&& - <and>",
	"&c - change to source path",
	"&C - change to target path",
	"&r - reload source directory",
	"&R - reload target directory",
	"&w - wait",
	NULL
};

static const char *PopNames[] =
{
	"ABOUT",
	"ACTION",
	"ALL",
	"ASSIGN",
	"BYNAME",
	"BYTES",
	"CHANGE",
	"COPY",
	"COPYAS",
	"COPYDEV",
	"CURRENT",
	"DELETE",
	"DISK",
	"DUP",
	"EXPAND",
	"FIT",
	"HELP",
	"ICON",
	"INFO",
	"LACTIVE",
	"LCOPY",
	"LISTARC",
	"LPARENT",
	"LRELOAD",
	"LROOT",
	"LSWAP",
	"MAKEDIR",
	"MDLOAD",
	"MOVE",
	"MOVEAS",
	"NONE",
	"NOTE",
	"PARENT",
	"PROTECT",
	"QUIT",
	"RACTIVE",
	"REBOOT",
	"RELABEL",
	"RELOAD",
	"RENAME",
	"ROOT",
	"RPARENT",
	"RRELOAD",
	"RROOT",
	"SETDATE",
	"SHRINK",
	"SWITCH",
	"TOGGLE",
	"TOUCH",
	"UNARC",
	"UPDATE",
	NULL
};

static const char *PopFormat[] =
{
	"C - Comment",
	"D - Date",
	"F - Flags",
	"N - Name",
	"S - Size",
	"T - Time",
	NULL
};

extern SAVEDS ASM LONG StrObjFunc(REG(a2) Object *pop,REG(a1) Object *str);
extern SAVEDS ASM VOID ObjStrFunc(REG(a2) Object *pop,REG(a1) Object *str);
extern SAVEDS ASM VOID WindowFunc(REG(a2) Object *pop,REG(a1) Object *win);
extern SAVEDS ASM VOID ObjStrFunc2(REG(a2) Object *pop,REG(a1) Object *str);
extern SAVEDS ASM VOID ObjStrFunc3(REG(a2) Object *pop,REG(a1) Object *str);

static const struct Hook StrObjHook = { { NULL,NULL },(VOID *)StrObjFunc,NULL,NULL };
static const struct Hook ObjStrHook = { { NULL,NULL },(VOID *)ObjStrFunc,NULL,NULL };
static const struct Hook WindowHook = { { NULL,NULL },(VOID *)WindowFunc,NULL,NULL };
static const struct Hook ObjStrHook2 = { { NULL,NULL },(VOID *)ObjStrFunc2,NULL,NULL };
static const struct Hook ObjStrHook3 = { { NULL,NULL },(VOID *)ObjStrFunc3,NULL,NULL };
