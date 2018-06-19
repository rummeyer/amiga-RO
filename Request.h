/*
**
** Request.h
**
*/

extern BOOL global_Special;

extern BOOL global_KeyFile;
extern LONG global_Actions;
extern BOOL cfg_QuitVerify;
extern BOOL global_QuitProgram;

extern APTR app_RumorOpus;
extern APTR wi_Main;
extern APTR wi_String;
extern APTR st_StringGadget;
extern APTR bt_StringOkay;
extern APTR bt_StringCancel;
extern APTR bt_StringSkip;
extern APTR bt_StringAll;

extern char global_Username[41];
extern char global_Userserial[5];

extern char * GetCatStr( int num, char *defstring );
extern void Sleep( BOOL Sleep_BOOL );
extern BOOL QuitRequester ( void );
extern BOOL CheckMemory ( int num );
extern void Fail ( void );
