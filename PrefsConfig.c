#include "PrefsIncludes.h"
#include "PrefsConfig.h"

/*
**
** DefaultConfig()
**
*/

void DefaultConfig( void )
{
	int i;

	set( cm_CheckFit, MUIA_Selected, TRUE);
	set( cm_FlushMemory, MUIA_Selected, FALSE);
	set( cm_CopyVerify, MUIA_Selected, FALSE);
	set( cm_CopyDate, MUIA_Selected, TRUE);
	set( cm_CopyFlags, MUIA_Selected, TRUE);
	set( cm_CopyNote, MUIA_Selected, TRUE);
	set( cm_CreateIcons, MUIA_Selected, TRUE);
	set( cm_CopyStat, MUIA_Selected, TRUE);
	set( cm_DelStat, MUIA_Selected, TRUE);
	set( cm_DelVerify, MUIA_Selected, TRUE);
	set( cm_DelForce, MUIA_Selected, FALSE);
	set( cm_PathExpand, MUIA_Selected, TRUE);
	set( cm_QuitVerify, MUIA_Selected, TRUE);
	set( cm_FollowScroll, MUIA_Selected, TRUE);
	set( cm_ShowDay, MUIA_Selected, TRUE);
	set( cm_ShowDate, MUIA_Selected, TRUE);
	set( cm_ShowTime, MUIA_Selected, TRUE);
	set( cm_ShowSeconds, MUIA_Selected, FALSE);
	set( cm_ShowMemory, MUIA_Selected, TRUE);
	set( cm_CheckDupes, MUIA_Selected, TRUE);
	set( cm_Symmetrical, MUIA_Selected, FALSE);

	set( cy_Overwrite, MUIA_Cycle_Active, 3);
	set( cy_PageLeft, MUIA_Cycle_Active, 0);
	set( cy_PageRight, MUIA_Cycle_Active, 0);
	set( cy_AppWindow, MUIA_Cycle_Active, 0);
	set( cy_Completed, MUIA_Cycle_Active, 0);
	set( cy_DriveMode, MUIA_Cycle_Active, 0);
	set( cy_HotMode, MUIA_Cycle_Active, 0);
	set( cy_FirstLeft, MUIA_Cycle_Active, 0);
	set( cy_SortLeft, MUIA_Cycle_Active, 0);
	set( cy_FirstRight, MUIA_Cycle_Active, 0);
	set( cy_SortRight, MUIA_Cycle_Active, 0);
	set( cy_MiddleMouse, MUIA_Cycle_Active, 0);

	set( st_TempDir, MUIA_String_Contents, "T:" );
	set( st_Output, MUIA_String_Contents, "CON:0/0/640/200/Output/CLOSE/AUTO" );
	set( st_History, MUIA_String_Contents, "50" );
	set( st_ClockPri, MUIA_String_Contents, "-10" );
	set( st_Refresh, MUIA_String_Contents, "500" );
	set( st_Buffer, MUIA_String_Contents, "524288" );
	set( st_Delay, MUIA_String_Contents, "0" );
	set( st_LoadLeft, MUIA_String_Contents, "" );
	set( st_LoadRight, MUIA_String_Contents, "" );
	set( st_FormatLeft, MUIA_String_Contents, "NSFDTC" );
	set( st_FormatRight, MUIA_String_Contents, "NSFDTC" );
	set( bt_FormatLeft, MUIA_Disabled, TRUE );
	set( bt_FormatRight, MUIA_Disabled, TRUE );

	for (i=0; i<10; i++)
		set( st_HotDir[i], MUIA_String_Contents, "" );

	DoMethod( lv_Input, MUIM_List_Clear );
	set( pp_Function, MUIA_Disabled, TRUE );
	set( pp_Command, MUIA_Disabled, TRUE );
	set( pp_Directory, MUIA_Disabled, TRUE );
	set( cy_Event, MUIA_Disabled, TRUE );
	set( cy_Type, MUIA_Disabled, TRUE );

	DoMethod( lv_Filetypes, MUIM_List_Clear );

	DoMethod( lv_Filetypes, MUIM_List_InsertSingle, "Default", MUIV_List_Insert_Bottom );
	DoMethod( lv_Filetypes, MUIM_List_InsertSingle, "ASCII File", MUIV_List_Insert_Bottom );
	DoMethod( lv_Filetypes, MUIM_List_InsertSingle, "Binary File", MUIV_List_Insert_Bottom );
	set( lv_Filetypes, MUIA_List_Active, 0 );

	strcpy( cfg_FileType[0], "MultiView >NIL: %f" );
	strcpy( cfg_FileType[1], cfg_FileType[0] );
	strcpy( cfg_FileType[2], cfg_FileType[0] );

	for (i=0; i<100; i++)
	{
		cfg_RecogType[i]=FALSE;
		strcpy( cfg_RecogString[i], "" );
		strcpy( cfg_RecogHex[i], "" );
		strcpy( cfg_RecogCommand[i], "" );
		strcpy( cfg_RecogCommandB[i], "" );
	}

	for (i=0; i<220; i++)
	{
		cfg_Type[i]=FALSE;
		strcpy( cfg_String[i], "" );
		cfg_Event[i]=0;
	}
}

/*
**
** LoadConfig()
**
*/

BOOL LoadConfig( char * filename )
{
	FILE *file;
	char line[256], word[6][256];
	int pos[6], c, i=0, j, num=0, dummy, lastnum=0;
	int ButtonNum=0, DriveNum=0, HotNum=0, MenuNum=0;
	BOOL quote=FALSE, Comment=FALSE, bool, success=FALSE;

	file = fopen(filename, "rb");
	if(file)
	{
		CurrentType = 0;
		while((c=fgetc(file))!=EOF)
		{
			if(c==9) c=32;
			if(c==';') Comment=TRUE;
			if(c!=10)
			{
				if (i<255&&!Comment)
				{
					line[i] = c;
					if(line[i]=='"') if((i-1)>0) if(line[i-1]!='*') { quote=!quote; line[i]=' '; i--; }
					if(line[i]==' '&&(!quote)) {if((i-1)>0) if((line[i-1]!=' ')&&(num<6)) { pos[num]=i+1; num++; i++; }}
					else i++;
				}
			}
			else
			{
				Comment = FALSE;

				pos[num]=i+1; line[i]=0;
				if(quote) { if(num<6) pos[num]=i+1; quote=FALSE; }
				for(j=0;j<6;j++) strcpy(word[j],"");
				strmid(line,word[0],1,pos[0]-1);
				for(j=1;j<6;j++)
					if(pos[j]-pos[j-1]>0)
						strmid(line,word[j],pos[j-1]+1,(pos[j]-pos[j-1])-1);
					else
						strcpy(word[j],"");

				if(stricmp("VARIABLE",word[0])==0)
				{
					if((stricmp("TEMPDIR",word[1])==0)&&(strlen(word[2])<81))		{set( st_TempDir, MUIA_String_Contents, word[2] );}
					if((stricmp("OUTPUT",word[1])==0)&&(strlen(word[2])<81))		{set( st_Output, MUIA_String_Contents, word[2] );}
					if((stricmp("HISTORY",word[1])==0)&&(strlen(word[2])<81))		{set( st_History, MUIA_String_Contents, word[2] );}
					if((stricmp("CLOCKPRI",word[1])==0)&&(strlen(word[2])<81))		{set( st_ClockPri, MUIA_String_Contents, word[2] );}
					if((stricmp("CLOCKREFRESH",word[1])==0)&&(strlen(word[2])<81))	{set( st_Refresh, MUIA_String_Contents, word[2] );}
					if((stricmp("COPYBUF",word[1])==0)&&(strlen(word[2])<81))		{set( st_Buffer, MUIA_String_Contents, word[2] );}
					if((stricmp("DELDELAY",word[1])==0)&&(strlen(word[2])<81))		{set( st_Delay, MUIA_String_Contents, word[2] );}
					if((stricmp("LOADLEFT",word[1])==0)&&(strlen(word[2])<81))		{set( st_LoadLeft, MUIA_String_Contents, word[2] );}
					if((stricmp("LOADRIGHT",word[1])==0)&&(strlen(word[2])<81))		{set( st_LoadRight, MUIA_String_Contents, word[2] );}
					if((stricmp("FORMATLEFT",word[1])==0)&&(strlen(word[2])<7))		{set( st_FormatLeft, MUIA_String_Contents, word[2] ); if(strlen(word[2])>5) set( bt_FormatLeft, MUIA_Disabled, TRUE );}
					if((stricmp("FORMATRIGHT",word[1])==0)&&(strlen(word[2])<7))	{set( st_FormatRight, MUIA_String_Contents, word[2] ); if(strlen(word[2])>5) set( bt_FormatRight, MUIA_Disabled, TRUE );}
					if((stricmp("PAGELEFT",word[1])==0)&&(strlen(word[2])<11))		{if(stricmp("Directory",word[2])==0) dummy=0; if(stricmp("Buffers",word[2])==0) dummy=1; if(stricmp("Volumes",word[2])==0) dummy=2; set( cy_PageLeft, MUIA_Cycle_Active, dummy);}
					if((stricmp("PAGERIGHT",word[1])==0)&&(strlen(word[2])<11))		{if(stricmp("Directory",word[2])==0) dummy=0; if(stricmp("Buffers",word[2])==0) dummy=1; if(stricmp("Volumes",word[2])==0) dummy=2; set( cy_PageRight, MUIA_Cycle_Active, dummy);}
					if(stricmp("APPWINDOW",word[1])==0)		{if(stricmp("COPY",word[2])==0) dummy=1; if(stricmp("LOAD",word[2])==0) dummy=0; set( cy_AppWindow, MUIA_Cycle_Active, dummy);}
					if(stricmp("OVERWRITE",word[1])==0)		{if(stricmp("ALWAYS",word[2])==0) dummy=0; if(stricmp("NEVER",word[2])==0) dummy=1; if(stricmp("OLDER",word[2])==0) dummy=2; if(stricmp("VERIFY",word[2])==0) dummy=3; set( cy_Overwrite, MUIA_Cycle_Active, dummy);}
					if(stricmp("COMPLETED",word[1])==0)		{if(stricmp("FLASH",word[2])==0) dummy=0; if(stricmp("POPUP",word[2])==0) dummy=1; if(stricmp("NOTHING",word[2])==0) dummy=2; set( cy_Completed, MUIA_Cycle_Active, dummy);}
					if(stricmp("DRIVEBUTTONS",word[1])==0)	{if(stricmp("NORMAL",word[2])==0) dummy=0; if(stricmp("ACTIVE",word[2])==0) dummy=1; set( cy_DriveMode, MUIA_Cycle_Active, dummy);}
					if(stricmp("HOTDIRS",word[1])==0)		{if(stricmp("NORMAL",word[2])==0) dummy=0; if(stricmp("ACTIVE",word[2])==0) dummy=1; set( cy_HotMode, MUIA_Cycle_Active, dummy);}
					if(stricmp("FIRSTLEFT",word[1])==0)		{if(stricmp("Dirs",word[2])==0) dummy=0; if(stricmp("Files",word[2])==0) dummy=1; if(stricmp("Mixed",word[2])==0) dummy=2; set( cy_FirstLeft, MUIA_Cycle_Active, dummy);}
					if(stricmp("FIRSTRIGHT",word[1])==0)	{if(stricmp("Dirs",word[2])==0) dummy=0; if(stricmp("Files",word[2])==0) dummy=1; if(stricmp("Mixed",word[2])==0) dummy=2; set( cy_FirstRight, MUIA_Cycle_Active, dummy);}
					if(stricmp("SORTLEFT",word[1])==0)		{if(stricmp("Name",word[2])==0) dummy=0; if(stricmp("Date",word[2])==0) dummy=1; if(stricmp("Size",word[2])==0) dummy=2; set( cy_SortLeft, MUIA_Cycle_Active, dummy);}
					if(stricmp("SORTRIGHT",word[1])==0)		{if(stricmp("Name",word[2])==0) dummy=0; if(stricmp("Date",word[2])==0) dummy=1; if(stricmp("Size",word[2])==0) dummy=2; set( cy_SortRight, MUIA_Cycle_Active, dummy);}
					if(stricmp("MIDDLEMOUSE",word[1])==0)	{if(stricmp("BOTH",word[2])==0) dummy=0; if(stricmp("LIST",word[2])==0) dummy=1; if(stricmp("BANK",word[2])==0) dummy=2; if(stricmp("NONE",word[2])==0) dummy=3; set( cy_MiddleMouse, MUIA_Cycle_Active, dummy);}
					if(stricmp("CHECKFIT",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_CheckFit, MUIA_Selected, bool); }
					if(stricmp("FLUSHMEMORY",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_FlushMemory, MUIA_Selected, bool); }
					if(stricmp("COPYVERIFY",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_CopyVerify, MUIA_Selected, bool); }
					if(stricmp("COPYDATE",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_CopyDate, MUIA_Selected, bool); }
					if(stricmp("COPYFLAGS",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_CopyFlags, MUIA_Selected, bool); }
					if(stricmp("COPYNOTE",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_CopyNote, MUIA_Selected, bool); }
					if(stricmp("CREATEICONS",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_CreateIcons, MUIA_Selected, bool); }
					if(stricmp("COPYSTAT",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_CopyStat, MUIA_Selected, bool); }
					if(stricmp("DELSTAT",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_DelStat, MUIA_Selected, bool); }
					if(stricmp("DELVERIFY",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_DelVerify, MUIA_Selected, bool); }
					if(stricmp("DELFORCE",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_DelForce, MUIA_Selected, bool); }
					if(stricmp("PATHEXPAND",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_PathExpand, MUIA_Selected, bool); }
					if(stricmp("QUITVERIFY",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_QuitVerify, MUIA_Selected, bool); }
					if(stricmp("FOLLOWSCROLL",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_FollowScroll, MUIA_Selected, bool); }
					if(stricmp("SHOWDAY",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_ShowDay, MUIA_Selected, bool); }
					if(stricmp("SHOWDATE",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_ShowDate, MUIA_Selected, bool); }
					if(stricmp("SHOWTIME",word[1])==0)		{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_ShowTime, MUIA_Selected, bool); }
					if(stricmp("SHOWSECONDS",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_ShowSeconds, MUIA_Selected, bool); }
					if(stricmp("SHOWMEMORY",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_ShowMemory, MUIA_Selected, bool); }
					if(stricmp("CHECKDUPES",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_CheckDupes, MUIA_Selected, bool); }
					if(stricmp("SYMMETRICAL",word[1])==0)	{if(stricmp("ON",word[2])==0) bool=TRUE; if(stricmp("OFF",word[2])==0) bool=FALSE; set( cm_Symmetrical, MUIA_Selected, bool); }
				}

				if((stricmp("HOTDIR",word[0])==0)&&(strlen(word[2])<81)&&(strlen(word[2])>0))
				{
					i=strtol(word[1],NULL,10);
					if(i>0&&i<11)
						set( st_HotDir[i-1], MUIA_String_Contents, word[2] );
				}

				if(stricmp("HOTKEY",word[0])==0)
				{
					if((strlen(word[1])>0)&&(strlen(word[3])>0)&&(strlen(word[1])<41)&&(strlen(word[3])<81)&&(HotNum<50))
					{
						HotNum++;
						cfg_Event[lastnum]=0;
						if(stricmp("COMMAND",word[2])==0) cfg_Type[lastnum]=TRUE;
						else cfg_Type[lastnum]=FALSE;
						DoMethod( lv_Input, MUIM_List_InsertSingle, word[1], MUIV_List_Insert_Bottom );
						strcpy(cfg_String[lastnum], word[3] );
						lastnum++;
					}
				}

				if(stricmp("BUTTON",word[0])==0)
				{
					if((strlen(word[4])>0)&&(strlen(word[3])<81)&&(strlen(word[4])<81)&&(ButtonNum<120))
					{
						ButtonNum++;
						cfg_Event[lastnum]=1;
						if(stricmp("COMMAND",word[2])==0) cfg_Type[lastnum]=TRUE;
						else cfg_Type[lastnum]=FALSE;
						DoMethod( lv_Input, MUIM_List_InsertSingle, word[3], MUIV_List_Insert_Bottom );
						strcpy(cfg_String[lastnum], word[4] );
						lastnum++;
					}
				}

				if(stricmp("DRIVE",word[0])==0)
				{
					if((strlen(word[2])>0)&&(strlen(word[3])>0)&&(strlen(word[2])<81)&&(strlen(word[3])<81)&&(DriveNum<32))
					{
						DriveNum++;
						cfg_Event[lastnum]=2;
						DoMethod( lv_Input, MUIM_List_InsertSingle, word[2], MUIV_List_Insert_Bottom );
						strcpy(cfg_String[lastnum], word[3] );
						lastnum++;
					}
				}

				if(stricmp("MENU",word[0])==0)
				{
					if((strlen(word[1])>0)&&(strlen(word[2])>0)&&(strlen(word[1])<41)&&(strlen(word[2])<81)&&(MenuNum<20))
					{
						MenuNum++;
						cfg_Event[lastnum]=3;
						DoMethod( lv_Input, MUIM_List_InsertSingle, word[1], MUIV_List_Insert_Bottom );
						strcpy(cfg_String[lastnum], word[2] );
						cfg_Type[lastnum]=TRUE;
						lastnum++;
					}
				}

				if(stricmp("BUFFER",word[0])==0)
				{
					if((strlen(word[1])>0)&&(strlen(word[1])<81))
					{
						cfg_Event[lastnum]=4;
						DoMethod( lv_Input, MUIM_List_InsertSingle, word[1], MUIV_List_Insert_Bottom );
						lastnum++;
					}
				}

				if(stricmp("FILETYPE",word[0])==0)
				{
					if((strlen(word[2])<81)&&(strlen(word[2])>0))
					{
						if(stricmp("ASCII",word[1])==0) strcpy(cfg_FileType[1],word[2]);
						if(stricmp("BINARY",word[1])==0) strcpy(cfg_FileType[2],word[2]);
						if(stricmp("DEFAULT",word[1])==0) strcpy(cfg_FileType[0],word[2]);
					}
				}

				if(stricmp("CUSTOMTYPE",word[0])==0)
				{
					if( strlen(word[1])<41 && strlen(word[3])<41 && strlen(word[2])<41 && strlen(word[4])<81 && (CurrentType<100))
					{
						DoMethod( lv_Filetypes, MUIM_List_InsertSingle, word[3], MUIV_List_Insert_Bottom );
						cfg_RecogType[CurrentType]=FALSE;
						strcpy( cfg_RecogString[CurrentType], word[1] );
						strcpy( cfg_RecogHex[CurrentType], word[2] );
						strcpy( cfg_RecogCommand[CurrentType], word[4] );
						CurrentType++;
					}
				}

				if(stricmp("ARCHIVETYPE",word[0])==0)
				{
					if( strlen(word[1])<41 && strlen(word[2])<41 && strlen(word[3])<41 && strlen(word[4])<81 && strlen(word[5])<81 && (CurrentType<100))
					{
						DoMethod( lv_Filetypes, MUIM_List_InsertSingle, word[3], MUIV_List_Insert_Bottom );
						cfg_RecogType[CurrentType]=TRUE;
						strcpy( cfg_RecogString[CurrentType], word[1] );
						strcpy( cfg_RecogHex[CurrentType], word[2] );
						strcpy( cfg_RecogCommand[CurrentType], word[4] );
						strcpy( cfg_RecogCommandB[CurrentType], word[5] );
						CurrentType++;
					}
				}

				i = 0;
				num = 0;
			}
		}
		fclose(file);
		set( lv_Filetypes, MUIA_List_Active, 0 );
		set( lv_Input, MUIA_List_Active, 0 );
		if ( lastnum > 0 )
			set( cy_Event, MUIA_Disabled, FALSE );
		success = TRUE;
	}
	return ( success );
}

/*
**
** Bool2String()
**
*/

char * GetSwitch( APTR obj )
{
	LONG State;

	get( obj, MUIA_Selected, &State );

	if (State)
		return( "ON" );
	else
		return( "OFF" );
}

/*
**
** SaveConfig()
**
*/

BOOL SaveConfig( char * filename )
{
	FILE *file;	
	char string[512], * TypeName, * str;
	int i, dummy, num;

	file = fopen( filename, "wb" );
	if ( file )
	{
		sprintf( string, "FILETYPE ASCII \"%s\"\n", cfg_FileType[1] );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "FILETYPE BINARY \"%s\"\n", cfg_FileType[2] );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "FILETYPE DEFAULT \"%s\"\n", cfg_FileType[0] );
		fwrite( string, 1, strlen(string), file );

		get( lv_Filetypes, MUIA_List_Entries, &CurrentType );
		for( i=3; i<CurrentType; i++ )
		{
			DoMethod( lv_Filetypes, MUIM_List_GetEntry, i, &TypeName );
			if( !cfg_RecogType[i-3] )
				sprintf( string, "CUSTOMTYPE \"%s\" \"%s\" \"%s\" \"%s\"\n", cfg_RecogString[i-3], cfg_RecogHex[i-3], TypeName, cfg_RecogCommand[i-3] );
			else
				sprintf( string, "ARCHIVETYPE \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n", cfg_RecogString[i-3], cfg_RecogHex[i-3], TypeName, cfg_RecogCommand[i-3], cfg_RecogCommandB[i-3] );
			fwrite( string, 1, strlen(string), file );
		}

		sprintf( string, "VARIABLE CHECKDUPES \"%s\"\n", GetSwitch(cm_CheckDupes) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE CREATEICONS \"%s\"\n", GetSwitch(cm_CreateIcons) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE FLUSHMEMORY \"%s\"\n", GetSwitch(cm_FlushMemory) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE FOLLOWSCROLL \"%s\"\n", GetSwitch(cm_FollowScroll) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE PATHEXPAND \"%s\"\n", GetSwitch(cm_PathExpand) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE QUITVERIFY \"%s\"\n", GetSwitch(cm_QuitVerify) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE SYMMETRICAL \"%s\"\n", GetSwitch(cm_Symmetrical) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE SHOWDAY \"%s\"\n", GetSwitch(cm_ShowDay) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE SHOWDATE \"%s\"\n", GetSwitch(cm_ShowDate) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE SHOWTIME \"%s\"\n", GetSwitch(cm_ShowTime) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE SHOWSECONDS \"%s\"\n", GetSwitch(cm_ShowSeconds) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE SHOWMEMORY \"%s\"\n", GetSwitch(cm_ShowMemory) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE DELVERIFY \"%s\"\n", GetSwitch(cm_DelVerify) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE DELFORCE \"%s\"\n", GetSwitch(cm_DelForce) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE DELSTAT \"%s\"\n", GetSwitch(cm_DelStat) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE CHECKFIT \"%s\"\n", GetSwitch(cm_CheckFit) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE COPYDATE \"%s\"\n", GetSwitch(cm_CopyDate) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE COPYFLAGS \"%s\"\n", GetSwitch(cm_CopyFlags) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE COPYNOTE \"%s\"\n", GetSwitch(cm_CopyNote) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE COPYSTAT \"%s\"\n", GetSwitch(cm_CopyStat) );
		fwrite( string, 1, strlen(string), file );
		sprintf( string, "VARIABLE COPYVERIFY \"%s\"\n", GetSwitch(cm_CopyVerify) );
		fwrite( string, 1, strlen(string), file );
		get( st_ClockPri, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE CLOCKPRI \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( st_Refresh, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE CLOCKREFRESH \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( st_Buffer, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE COPYBUF \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( st_Delay, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE DELDELAY \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( st_History, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE HISTORY \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( st_Output, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE OUTPUT \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( st_TempDir, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE TEMPDIR \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( cy_AppWindow, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE APPWINDOW \"%s\"\n", CYA_AppWindow[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_DriveMode, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE DRIVEBUTTONS \"%s\"\n", CYA_Mode[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_HotMode, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE HOTDIRS \"%s\"\n", CYA_Mode[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_Completed, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE COMPLETED \"%s\"\n", CYA_Completed[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_MiddleMouse, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE MIDDLEMOUSE \"%s\"\n", CYA_MiddleMouse[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_Overwrite, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE OVERWRITE \"%s\"\n", CYA_Overwrite[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_PageLeft, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE PAGELEFT \"%s\"\n", CYA_Pages[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_PageRight, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE PAGERIGHT \"%s\"\n", CYA_Pages[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_FirstLeft, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE FIRSTLEFT \"%s\"\n", CYA_First[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_FirstRight, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE FIRSTRIGHT \"%s\"\n", CYA_First[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_SortLeft, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE SORTLEFT \"%s\"\n", CYA_Sort[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( cy_SortRight, MUIA_Cycle_Active, &dummy );
		sprintf( string, "VARIABLE SORTRIGHT \"%s\"\n", CYA_Sort[dummy] );
		fwrite( string, 1, strlen(string), file );
		get( st_LoadLeft, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE LOADLEFT \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( st_LoadRight, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE LOADRIGHT \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( st_FormatLeft, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE FORMATLEFT \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );
		get( st_FormatRight, MUIA_String_Contents, &str );
		sprintf( string, "VARIABLE FORMATRIGHT \"%s\"\n", str );
		fwrite( string, 1, strlen(string), file );

		for( i=0; i<10; i++ )
		{
			get( st_HotDir[i], MUIA_String_Contents, &str );
			if( strlen( str ) > 0 )
			{
				sprintf( string, "HOTDIR %d \"%s\"\n", i+1, str );
				fwrite( string, 1, strlen(string), file );
			}
		}

		num = 0;
		get( lv_Input, MUIA_List_Entries, &dummy );
		for( i=0; i<dummy; i++ )
		{
			if( cfg_Event[i]==3 )
			{
				num++;
				if(num>20) break;
				DoMethod( lv_Input, MUIM_List_GetEntry, i, &TypeName );
				sprintf( string, "MENU \"%s\" \"%s\"\n", TypeName, cfg_String[i] );
				fwrite( string, 1, strlen(string), file );
			}
		}

		num = 0;
		get( lv_Input, MUIA_List_Entries, &dummy );
		for( i=0; i<dummy; i++ )
		{
			if( cfg_Event[i]==0 )
			{
				num++;
				if(num>50) break;
				DoMethod( lv_Input, MUIM_List_GetEntry, i, &TypeName );
				if( cfg_Type[i] )
					sprintf( string, "HOTKEY \"%s\" COMMAND \"%s\"\n", TypeName, cfg_String[i] );
				else
					sprintf( string, "HOTKEY \"%s\" FUNCTION \"%s\"\n", TypeName, cfg_String[i] );
				fwrite( string, 1, strlen(string), file );
			}
		}

		get( lv_Input, MUIA_List_Entries, &dummy );
		for( i=0; i<dummy; i++ )
		{
			if( cfg_Event[i]==4 )
			{
				DoMethod( lv_Input, MUIM_List_GetEntry, i, &TypeName );
				sprintf( string, "BUFFER \"%s\"\n", TypeName );
				fwrite( string, 1, strlen(string), file );
			}
		}

		num=0;
		get( lv_Input, MUIA_List_Entries, &dummy );
		for( i=0; i<dummy; i++ )
		{
			if( cfg_Event[i]==2 )
			{
				if(num>32) break;
				DoMethod( lv_Input, MUIM_List_GetEntry, i, &TypeName );
				sprintf( string, "DRIVE %d \"%s\" \"%s\"\n", ++num, TypeName, cfg_String[i] );
				fwrite( string, 1, strlen(string), file );
			}
		}

		num=0;
		get( lv_Input, MUIA_List_Entries, &dummy );
		for( i=0; i<dummy; i++ )
		{
			if( cfg_Event[i]==1 )
			{
				if(num>120) break;
				DoMethod( lv_Input, MUIM_List_GetEntry, i, &TypeName );
				if( cfg_Type[i] )
					sprintf( string, "BUTTON %d COMMAND \"%s\" \"%s\"\n", ++num, TypeName, cfg_String[i] );
				else
					sprintf( string, "BUTTON %d FUNCTION \"%s\" \"%s\"\n", ++num, TypeName, cfg_String[i] );
				fwrite( string, 1, strlen(string), file );
			}
		}

		fclose( file );
		return( TRUE );
	}
	else
		return( FALSE );
}
