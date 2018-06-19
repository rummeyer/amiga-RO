/*
**
** PrefsMain.h
**
*/

BOOL global_Special = TRUE;

extern APTR app_RumorOpusPrefs;
extern APTR wi_Main;

extern BOOL cfg_CheckFit;
extern BOOL global_KeyFile;

BOOL global_Cancel=FALSE;
BOOL global_QuitProgram=FALSE;

extern void Fail ( void );
extern void Init ( void );
extern void CheckKey ( void );
extern BOOL LoadConfig( char * filename );
extern BOOL SaveConfig( char * filename );
extern void DefaultConfig( void );

extern BOOL cfg_Type[220];
extern char cfg_String[220][81];
extern int cfg_Event[220];
extern APTR st_Directory;
extern APTR st_Input;
extern APTR st_Command;
extern APTR pp_Command;
extern APTR st_Function;
extern APTR pp_Function;
extern APTR cy_Type;
extern APTR lv_Input;
extern APTR cy_Event;
extern APTR lv_Filetypes;
extern APTR st_Filetypes;
extern APTR cy_Filetype;
extern APTR bt_DelFiletype;
extern APTR bt_AddFiletype;
extern APTR bt_AddEvent;
extern APTR st_CommandA;
extern APTR pp_Directory;
extern char cfg_FileType[3][81];
extern APTR st_CommandB;
extern APTR st_Pattern;
extern APTR st_Recog;
extern APTR bt_Save;
extern APTR bt_Use;
extern APTR bt_FormatLeft;
extern APTR bt_FormatRight;
extern APTR st_FormatLeft;
extern APTR st_FormatRight;
extern APTR pp_CommandA;
extern APTR pp_CommandB;

extern BOOL global_KeyFile;

extern __far BOOL cfg_RecogType[100];
extern __far char cfg_RecogString[100][41];
extern __far char cfg_RecogHex[100][41];
extern __far char cfg_RecogCommand[100][81];
extern __far char cfg_RecogCommandB[100][81];

extern BOOL AboutRequester ( void );
