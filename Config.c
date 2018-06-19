#include "Includes.h"
#include "Config.h"

/*
**
** DefaultConfig()
**
*/

void DefaultConfig ( void )
{
	int i;

	/*** Standard Config ***/

	for ( i = 0; i < 100; i++ )
	{
		strcpy( cfg_RecogString[i], "" );
		strcpy( cfg_RecogHex[i], "" );
		strcpy( cfg_RecogName[i], "" );
		strcpy( cfg_RecogCommand[i], "" );
		strcpy( cfg_RecogCommandB[i], "" );
	}

	for( i = 0; i < 32; i++ )
	{
		strcpy(cfg_DriveText[i],"");
		strcpy(cfg_Drive[i],"");
	}

	cfg_CheckFit				= TRUE;
	cfg_ClockPri				= -10;
	cfg_ClockRefresh			= 500;
	cfg_Completed				= 1;
	cfg_CopyBuf					= 524288;
	cfg_CopyDate				= TRUE;
	cfg_CopyFlags				= TRUE;
	cfg_CopyNote				= TRUE;
	cfg_CopyStat				= TRUE;
	cfg_CopyVerify				= FALSE;
	cfg_CreateIcons				= TRUE;
	cfg_DelDelay				= 0;
	cfg_DelForce				= 0;
	cfg_DelStat         		= TRUE;
	cfg_DelVerify				= TRUE;
	cfg_DriveButtons			= FALSE;
	cfg_DropIcons				= FALSE;
	cfg_FirstType[Left_Side]	= MUIV_Dirlist_SortDirs_First;
	cfg_FirstType[Right_Side]	= MUIV_Dirlist_SortDirs_First;
	cfg_SortHighLow[Left_Side]	= FALSE;
	cfg_SortHighLow[Right_Side]	= FALSE;
	cfg_History         		= 50;
	cfg_HotDirs					= FALSE;
	cfg_Overwrite				= 2;
	cfg_PathExpand				= TRUE;
	cfg_QuitVerify				= TRUE;
	cfg_Scroll					= TRUE;
	cfg_ShowClock				= TRUE;
	cfg_ShowSeconds				= FALSE;
	cfg_ShowDate				= TRUE;
	cfg_ShowDay					= TRUE;
	cfg_ShowMem					= TRUE;
	cfg_CheckDupes              = TRUE;
	cfg_SortType[Left_Side]		= MUIV_Dirlist_SortType_Name;
	cfg_SortType[Right_Side]	= MUIV_Dirlist_SortType_Name;
	cfg_LeftPage				= 0;
	cfg_RightPage				= 0;
	cfg_FlushMemory				= FALSE;
	cfg_MiddleMouse				= 3;
	cfg_MultiSelect				= MUIV_Listview_MultiSelect_Always;
	cfg_PageType				= TRUE;

	strcpy( cfg_LeftLoad,       "" );
	strcpy( cfg_RightLoad,      "" );
	strcpy( cfg_Format[Left_Side],     "COL=0, COL=1 P=\33r, COL=4, COL=2, COL=3, COL=5" );
	strcpy( cfg_Format[Right_Side],    cfg_Format[Left_Side] );
	strcpy( cfg_Output,         "CON:0/0/640/200/Output/CLOSE/AUTO" );
	strcpy( cfg_TempDir,        "T:" );

	strcpy( cfg_FileType[0],    "MultiView >NIL: %f" );
	strcpy( cfg_FileType[1],    cfg_FileType[0] );
	strcpy( cfg_FileType[2],    cfg_FileType[0] );

	strcpy( cfg_HotDir[0], "" );
	strcpy( cfg_HotDir[1], "" );
	strcpy( cfg_HotDir[2], "" );
	strcpy( cfg_HotDir[3], "" );
	strcpy( cfg_HotDir[4], "" );
	strcpy( cfg_HotDir[5], "" );
	strcpy( cfg_HotDir[6], "" );
	strcpy( cfg_HotDir[7], "" );
	strcpy( cfg_HotDir[8], "" );
	strcpy( cfg_HotDir[9], "" );

	global_MaximumBank = 1;

	cfg_ButtonType[0]	= FALSE;
	cfg_ButtonType[1]	= FALSE;
	cfg_ButtonType[2]	= TRUE;
	cfg_ButtonType[3]	= TRUE;
	cfg_ButtonType[4]	= TRUE;
	cfg_ButtonType[5]	= TRUE;
	cfg_ButtonType[6]	= TRUE;
	cfg_ButtonType[7]	= FALSE;
	cfg_ButtonType[8]	= FALSE;
	cfg_ButtonType[9]	= FALSE;
	cfg_ButtonType[10]	= TRUE;
	cfg_ButtonType[11]	= TRUE;
	cfg_ButtonType[12]	= TRUE;
	cfg_ButtonType[13]	= TRUE;
	cfg_ButtonType[14]	= TRUE;
	cfg_ButtonType[15]	= TRUE;
	cfg_ButtonType[16]	= TRUE;
	cfg_ButtonType[17]	= FALSE;
	cfg_ButtonType[18]	= TRUE;
	cfg_ButtonType[19]	= TRUE;
	cfg_ButtonType[20]	= TRUE;
	cfg_ButtonType[21]	= TRUE;
	cfg_ButtonType[22]	= TRUE;
	cfg_ButtonType[23]	= TRUE;

	cfg_ButtonType[24]	= FALSE;
	cfg_ButtonType[25]	= TRUE;
	cfg_ButtonType[26]	= FALSE;
	cfg_ButtonType[27]	= TRUE;
	cfg_ButtonType[28]	= TRUE;
	cfg_ButtonType[29]	= TRUE;
	cfg_ButtonType[30]	= TRUE;
	cfg_ButtonType[31]	= TRUE;
	cfg_ButtonType[32]	= TRUE;
	cfg_ButtonType[33]	= TRUE;
	cfg_ButtonType[34]	= FALSE;
	cfg_ButtonType[35]	= TRUE;
	cfg_ButtonType[36]	= TRUE;
	cfg_ButtonType[37]	= TRUE;
	cfg_ButtonType[38]	= TRUE;
	cfg_ButtonType[39]	= TRUE;
	cfg_ButtonType[40]	= TRUE;
	cfg_ButtonType[41]	= TRUE;
	cfg_ButtonType[42]	= FALSE;
	cfg_ButtonType[43]	= TRUE;
	cfg_ButtonType[44]	= TRUE;
	cfg_ButtonType[45]	= TRUE;
	cfg_ButtonType[46]	= TRUE;
	cfg_ButtonType[47]	= TRUE;

	strcpy( cfg_ButtonText[0],     "Read" );
	strcpy( cfg_ButtonText[1],     "Run" );
	strcpy( cfg_ButtonText[2],     "_Delete" );
	strcpy( cfg_ButtonText[3],     "_Move" );
	strcpy( cfg_ButtonText[4],     "_Copy" );
	strcpy( cfg_ButtonText[5],     "_Bytes" );
	strcpy( cfg_ButtonText[6],     "All" );
	strcpy( cfg_ButtonText[7],     "Arc" );
	strcpy( cfg_ButtonText[8],     "View" );
	strcpy( cfg_ButtonText[9],     "_Edit" );
	strcpy( cfg_ButtonText[10],    "Touch" );
	strcpy( cfg_ButtonText[11],    "Move As" );
	strcpy( cfg_ButtonText[12],    "Copy As" );
	strcpy( cfg_ButtonText[13],    "_Fit" );
	strcpy( cfg_ButtonText[14],    "None" );
	strcpy( cfg_ButtonText[15],    "ListArc" );
	strcpy( cfg_ButtonText[16],    "_Action" );
	strcpy( cfg_ButtonText[17],    "XEdit" );
	strcpy( cfg_ButtonText[18],    "MakeDir" );
	strcpy( cfg_ButtonText[19],    "_Rename" );
	strcpy( cfg_ButtonText[20],    "Dup" );
	strcpy( cfg_ButtonText[21],    "Refresh" );
	strcpy( cfg_ButtonText[22],    "_Pattern" );
	strcpy( cfg_ButtonText[23],    "UnArc" );

	strcpy( cfg_ButtonText[24],    "Music" );
	strcpy( cfg_ButtonText[25],    "Assign" );
	strcpy( cfg_ButtonText[26],    "Command" );
	strcpy( cfg_ButtonText[27],    "_Comment" );
	strcpy( cfg_ButtonText[28],    "Change" );
	strcpy( cfg_ButtonText[29],    "Parent" );
	strcpy( cfg_ButtonText[30],    "Reverse" );
	strcpy( cfg_ButtonText[31],    "_Disk" );
	strcpy( cfg_ButtonText[32],    "LSwap" );
	strcpy( cfg_ButtonText[33],    "_Relabel" );
	strcpy( cfg_ButtonText[34],    "Execute" );
	strcpy( cfg_ButtonText[35],    "_Protect" );
	strcpy( cfg_ButtonText[36],    "_Expand" );
	strcpy( cfg_ButtonText[37],    "Root" );
	strcpy( cfg_ButtonText[38],    "_Update" );
	strcpy( cfg_ButtonText[39],    "_Info" );
	strcpy( cfg_ButtonText[40],    "LCopy" );
	strcpy( cfg_ButtonText[41],    "CopyDev" );
	strcpy( cfg_ButtonText[42],    "Rexx" );
	strcpy( cfg_ButtonText[43],    "SetDate" );
	strcpy( cfg_ButtonText[44],    "_Shrink" );
	strcpy( cfg_ButtonText[45],    "Current" );
	strcpy( cfg_ButtonText[46],    "Icon" );
	strcpy( cfg_ButtonText[47],    "Buffers" );

	strcpy( cfg_ButtonCommand[0],  "Most >NIL: %f" );
	strcpy( cfg_ButtonCommand[1],  "Run >NIL: %f" );
	strcpy( cfg_ButtonCommand[2],  "DELETE" );
	strcpy( cfg_ButtonCommand[3],  "MOVE" );
	strcpy( cfg_ButtonCommand[4],  "COPY" );
	strcpy( cfg_ButtonCommand[5],  "BYTES" );
	strcpy( cfg_ButtonCommand[6],  "ALL" );
	strcpy( cfg_ButtonCommand[7],  "LhA >NIL: -x -r a %R %n &c" );
	strcpy( cfg_ButtonCommand[8],  "MultiView >NIL: %f" );
	strcpy( cfg_ButtonCommand[9],  "Ed >NIL: %f" );
	strcpy( cfg_ButtonCommand[10], "TOUCH" );
	strcpy( cfg_ButtonCommand[11], "MOVEAS" );
	strcpy( cfg_ButtonCommand[12], "COPYAS" );
	strcpy( cfg_ButtonCommand[13], "FIT" );
	strcpy( cfg_ButtonCommand[14], "NONE" );
	strcpy( cfg_ButtonCommand[15], "LISTARC" );
	strcpy( cfg_ButtonCommand[16], "ACTION" );
	strcpy( cfg_ButtonCommand[17], "FileX >NIL: %f" );
	strcpy( cfg_ButtonCommand[18], "MAKEDIR" );
	strcpy( cfg_ButtonCommand[19], "RENAME" );
	strcpy( cfg_ButtonCommand[20], "DUP" );
	strcpy( cfg_ButtonCommand[21], "RELOAD" );
	strcpy( cfg_ButtonCommand[22], "BYNAME" );
	strcpy( cfg_ButtonCommand[23], "UNARC" );

	strcpy( cfg_ButtonCommand[24], "DeliTracker >NIL: MODULE %f" );
	strcpy( cfg_ButtonCommand[25], "ASSIGN" );
	strcpy( cfg_ButtonCommand[26], "%r" );
	strcpy( cfg_ButtonCommand[27], "NOTE" );
	strcpy( cfg_ButtonCommand[28], "CHANGE" );
	strcpy( cfg_ButtonCommand[29], "PARENT" );
	strcpy( cfg_ButtonCommand[30], "TOGGLE" );
	strcpy( cfg_ButtonCommand[31], "DISK" );
	strcpy( cfg_ButtonCommand[32], "LSWAP" );
	strcpy( cfg_ButtonCommand[33], "RELABEL" );
	strcpy( cfg_ButtonCommand[34], "Execute %r" );
	strcpy( cfg_ButtonCommand[35], "PROTECT" );
	strcpy( cfg_ButtonCommand[36], "EXPAND" );
	strcpy( cfg_ButtonCommand[37], "ROOT" );
	strcpy( cfg_ButtonCommand[38], "UPDATE" );
	strcpy( cfg_ButtonCommand[39], "INFO" );
	strcpy( cfg_ButtonCommand[40], "LCOPY" );
	strcpy( cfg_ButtonCommand[41], "COPYDEV" );
	strcpy( cfg_ButtonCommand[42], "Rx %r" );
	strcpy( cfg_ButtonCommand[43], "SETDATE" );
	strcpy( cfg_ButtonCommand[44], "SHRINK" );
	strcpy( cfg_ButtonCommand[45], "CURRENT" );
	strcpy( cfg_ButtonCommand[46], "ICON" );
	strcpy( cfg_ButtonCommand[47], "BUFFERS" );
}

/*
**
** LoadConfig()
**
*/

BOOL LoadConfig( BOOL FirstPass )
{
	FILE *file;
	char line[256], word[6][256], bufferstring[512], Format_String[81], Help_String[3];
	int pos[6], c, i=0, j, n, l, num=0, menucount=0, CurrentType=0, number, LineNumber=0;
	BOOL quote=FALSE, buttonfound=FALSE, drivefound=FALSE, moredrives=FALSE, custommenu=FALSE, Comment=FALSE, Error=FALSE;

	file = fopen("ENV:RO/RO.prefs","rb");
	if(!file) file = fopen("S:RO.prefs","rb");
	if(!file) file = fopen("PROGDIR:RO.prefs","rb");

	if(file)
	{
		while((c=fgetc(file))!=EOF)
		{
			if(c==9) c=32;
			if(c=='^') c=27;
			if(c==';') Comment=TRUE;
			if(c!=10)
			{
				if (i<255&&!Comment)
				{
					line[i] = c;
					if(line[i]=='"') if((i-1)>0) if(line[i-1]!='*') { quote=!quote; line[i]=' '; i--; } else {line[i-1]='"'; line[i]=' '; i--;}
					if(line[i]==' '&&(!quote)) {if((i-1)>0) if((line[i-1]!=' ')&&(num<6)) { pos[num]=i+1; num++; i++; }}
					else i++;
				}
			}
			else
			{
				Error = FALSE;
				Comment = FALSE;
				LineNumber++;

				pos[num]=i+1; line[i]=0;
				if(quote) { if(num<6) pos[num]=i+1; quote=FALSE; }
				for(j=0;j<6;j++) strcpy(word[j],"");
				strmid(line,word[0],1,pos[0]-1);
				for(j=1;j<6;j++)
					if(pos[j]-pos[j-1]>0)
						strmid(line,word[j],pos[j-1]+1,(pos[j]-pos[j-1])-1);
					else
						strcpy(word[j],"");

				if ( !FirstPass )
				{

				if(stricmp("FILETYPE",word[0])==0)
				{
					if(strlen(word[2])<81)
					{
						if(stricmp("BINARY",word[1])==0) strcpy(cfg_FileType[0],word[2]);
						if(stricmp("ASCII",word[1])==0) strcpy(cfg_FileType[1],word[2]);
						if(stricmp("DEFAULT",word[1])==0) strcpy(cfg_FileType[2],word[2]);
					}
					else
						Error = TRUE;
				}

				if((stricmp("CUSTOMTYPE",word[0])==0)||(stricmp("ARCHIVETYPE",word[0])==0))
				{
					if(word[2][0]=='$')
					{
						n=0;
						i=0;
						l=strlen(word[2])-1;
						while( l > 0 )
						{
							Help_String[0]=word[2][++n];
							Help_String[1]=word[2][++n];
							Help_String[2]='\0';
							if(!((Help_String[0]=='?'||Help_String[1]=='?')))
							{
								number=strtol(Help_String,NULL,16);
								if(number!=0)
									word[2][i]=number;
								else
									word[2][i]=255;
							}
							else
								word[2][i]='?';
							l=l-2;
							i++;
						}
						word[2][i]='\0';
					}
				}

				if(stricmp("CUSTOMTYPE",word[0])==0)
				{
					if( strlen(word[1])<41 && strlen(word[3])<41 && strlen(word[2])<41 && strlen(word[4])<81 && CurrentType<100 )
					{
						sprintf( cfg_RecogString[CurrentType], "%s", word[1] );
						strcpy( cfg_RecogHex[CurrentType], word[2] );
						strcpy( cfg_RecogName[CurrentType], word[3] );
						strcpy( cfg_RecogCommand[CurrentType], word[4] );
						CurrentType++;
					}
					else
						Error = TRUE;
				}

				if(stricmp("ARCHIVETYPE",word[0])==0)
				{
					if( strlen(word[1])<41 && strlen(word[2])<41 && strlen(word[3])<41 && strlen(word[4])<81 && strlen(word[5])<81 && CurrentType<100 )
					{
						sprintf( cfg_RecogString[CurrentType], "%s", word[1] );
						strcpy( cfg_RecogHex[CurrentType], word[2] );
						strcpy( cfg_RecogName[CurrentType], word[3] );
						strcpy( cfg_RecogCommand[CurrentType], word[4] );
						strcpy( cfg_RecogCommandB[CurrentType], word[5] );
						CurrentType++;
					}
					else
						Error = TRUE;
				}

				if(stricmp("VARIABLE",word[0])==0)
				{
					if((stricmp("TEMPDIR",word[1])==0)&&(strlen(word[2])<81))		strcpy(cfg_TempDir,word[2]);
					if((stricmp("OUTPUT",word[1])==0)&&(strlen(word[2])<81))		strcpy(cfg_Output,word[2]);
					if((stricmp("PAGELEFT",word[1])==0)&&(strlen(word[2])<11))		{if(stricmp("Directory",word[2])==0) cfg_LeftPage=0; if(stricmp("Buffers",word[2])==0) cfg_LeftPage=1; if(stricmp("Volumes",word[2])==0) cfg_LeftPage=2;}
					if((stricmp("PAGERIGHT",word[1])==0)&&(strlen(word[2])<11))		{if(stricmp("Directory",word[2])==0) cfg_RightPage=0; if(stricmp("Buffers",word[2])==0) cfg_RightPage=1; if(stricmp("Volumes",word[2])==0) cfg_RightPage=2;}
					if((stricmp("LOADLEFT",word[1])==0)&&(strlen(word[2])<41))		strcpy(cfg_LeftLoad,word[2]);
					if((stricmp("LOADRIGHT",word[1])==0)&&(strlen(word[2])<41))		strcpy(cfg_RightLoad,word[2]);
					if(stricmp("APPWINDOW",word[1])==0)		{if(stricmp("COPY",word[2])==0) cfg_DropIcons=TRUE; if(stricmp("LOAD",word[2])==0) cfg_DropIcons=FALSE;}
					if(stricmp("CHECKFIT",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_CheckFit=TRUE; if(stricmp("OFF",word[2])==0) cfg_CheckFit=FALSE;}
					if(stricmp("OVERWRITE",word[1])==0)		{if(stricmp("ALWAYS",word[2])==0) cfg_Overwrite=0; if(stricmp("NEVER",word[2])==0) cfg_Overwrite=1; if(stricmp("VERIFY",word[2])==0) cfg_Overwrite=2; if(stricmp("OLDER",word[2])==0) cfg_Overwrite=3;}
					if(stricmp("COMPLETED",word[1])==0)		{if(stricmp("QUIET",word[2])==0) cfg_Completed=0; if(stricmp("NOTHING",word[2])==0) cfg_Completed=0; if(stricmp("FLASH",word[2])==0) cfg_Completed=1; if(stricmp("POPUP",word[2])==0) cfg_Completed=2;}
					if(stricmp("FLUSHMEMORY",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_FlushMemory=TRUE; if(stricmp("OFF",word[2])==0) cfg_FlushMemory=FALSE;}
					if(stricmp("HISTORY",word[1])==0)		{cfg_History=strtol(word[2],NULL,10); if(cfg_History<=0) cfg_History=50;}
					if(stricmp("COPYBUF",word[1])==0)		{cfg_CopyBuf=strtol(word[2],NULL,10); if(cfg_CopyBuf<=0) cfg_CopyBuf=1048576;}
					if(stricmp("COPYVERIFY",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_CopyVerify=TRUE; if(stricmp("OFF",word[2])==0) cfg_CopyVerify=FALSE;}
					if(stricmp("COPYDATE",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_CopyDate=TRUE; if(stricmp("OFF",word[2])==0) cfg_CopyDate=FALSE;}
					if(stricmp("COPYFLAGS",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_CopyFlags=TRUE; if(stricmp("OFF",word[2])==0) cfg_CopyFlags=FALSE;}
					if(stricmp("COPYNOTE",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_CopyNote=TRUE; if(stricmp("OFF",word[2])==0) cfg_CopyNote=FALSE;}
					if(stricmp("CREATEICONS",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_CreateIcons=TRUE; if(stricmp("OFF",word[2])==0) cfg_CreateIcons=FALSE;}
					if(stricmp("DRIVEBUTTONS",word[1])==0)	{if(stricmp("NORMAL",word[2])==0) cfg_DriveButtons=FALSE; if(stricmp("ACTIVE",word[2])==0) cfg_DriveButtons=TRUE;}
					if(stricmp("HOTDIRS",word[1])==0)		{if(stricmp("NORMAL",word[2])==0) cfg_HotDirs=FALSE; if(stricmp("ACTIVE",word[2])==0) cfg_HotDirs=TRUE;}
					if(stricmp("COPYSTAT",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_CopyStat=TRUE; if(stricmp("OFF",word[2])==0) cfg_CopyStat=FALSE;}
					if(stricmp("DELSTAT",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_DelStat=TRUE;	if(stricmp("OFF",word[2])==0) cfg_DelStat=FALSE;}
					if(stricmp("DELVERIFY",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_DelVerify=TRUE; if(stricmp("OFF",word[2])==0) cfg_DelVerify=FALSE;}
					if(stricmp("DELFORCE",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_DelForce=1; if(stricmp("OFF",word[2])==0) cfg_DelForce=0;}
					if(stricmp("DELDELAY",word[1])==0)		{cfg_DelDelay=strtol(word[2],NULL,10); if(cfg_DelDelay<0) cfg_DelDelay=0;}
					if(stricmp("PATHEXPAND",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_PathExpand=TRUE; if(stricmp("OFF",word[2])==0) cfg_PathExpand=FALSE;}
					if(stricmp("QUITVERIFY",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_QuitVerify=TRUE; if(stricmp("OFF",word[2])==0) cfg_QuitVerify=FALSE;}
					if(stricmp("FOLLOWSCROLL",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_Scroll=TRUE; if(stricmp("OFF",word[2])==0) cfg_Scroll=FALSE;}
					if(stricmp("SHOWDAY",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_ShowDay=TRUE; if(stricmp("OFF",word[2])==0) cfg_ShowDay=FALSE;}
					if(stricmp("SHOWDATE",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_ShowDate=TRUE; if(stricmp("OFF",word[2])==0) cfg_ShowDate=FALSE;}
					if(stricmp("SHOWTIME",word[1])==0)		{if(stricmp("ON",word[2])==0) cfg_ShowClock=TRUE; if(stricmp("OFF",word[2])==0) cfg_ShowClock=FALSE;}
					if(stricmp("SHOWSECONDS",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_ShowSeconds=TRUE; if(stricmp("OFF",word[2])==0) cfg_ShowSeconds=FALSE;}
					if(stricmp("SHOWMEMORY",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_ShowMem=TRUE; if(stricmp("OFF",word[2])==0) cfg_ShowMem=FALSE;}
					if(stricmp("CLOCKPRI",word[1])==0)		{cfg_ClockPri=strtol(word[2],NULL,10); if((cfg_ClockPri>127)||(cfg_ClockPri<-128)) cfg_ClockPri = -10;}
					if(stricmp("CLOCKREFRESH",word[1])==0)	{cfg_ClockRefresh=strtol(word[2],NULL,10); if(cfg_ClockRefresh<0) cfg_ClockRefresh=50;}
					if(stricmp("CHECKDUPES",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_CheckDupes=TRUE; if(stricmp("OFF",word[2])==0) cfg_CheckDupes=FALSE;}
					if(stricmp("MIDDLEMOUSE",word[1])==0)	{if(stricmp("LIST",word[2])==0) cfg_MiddleMouse=1; if(stricmp("BANK",word[2])==0) cfg_MiddleMouse=2; if(stricmp("BOTH",word[2])==0) cfg_MiddleMouse=3; if(stricmp("NONE",word[2])==0) cfg_MiddleMouse=0;}
				}

				if(stricmp("MENU",word[0])==0)
				{
					if(strlen(stpblk(word[1]))>0)
					{
						if((strlen(word[1])<41)&&(strlen(word[2])<81)&&menucount<21)
						{
							if(!custommenu)
							{
								DoMethod(Menu,MUIM_Family_AddTail,men_UserMenu);
								custommenu=TRUE;
							}
							strcpy(cfg_MenuEntry[menucount],word[2]);
							strcpy(cfg_MenuEntryText[menucount],word[1]);
							DoMethod(men_UserMenu, MUIM_Family_AddTail, MUI_MakeObject(MUIO_Menuitem, cfg_MenuEntryText[menucount] ,0,0,ID_Menu_0+menucount));
							menucount++;
						}
						else
							Error = TRUE;
					}
				}

				if((stricmp("BUFFER",word[0])==0)&&(strlen(word[1])>0))
				{
						sprintf( bufferstring, "\33b%s", word[1] );
						AddPart( bufferstring, "", sizeof(bufferstring) );
						DoMethod(lv_Buffers[Left_Side], MUIM_List_InsertSingle, bufferstring, MUIV_List_Insert_Bottom);
						DoMethod(lv_Buffers[Right_Side], MUIM_List_InsertSingle, bufferstring, MUIV_List_Insert_Bottom);
						global_NumBuffers++;
				}

				if((stricmp("HOTDIR",word[0])==0)&&(strlen(word[2])<81))
				{
					i=strtol(word[1],NULL,10);
					if(i>0&&i<11) strcpy(cfg_HotDir[i-1],word[2]);
				}

				if(stricmp("DRIVE",word[0])==0)
				{
					if((strlen(word[2])<81)&&(strlen(word[3])<81))
					{
						drivefound=TRUE;
						i=strtol(word[1],NULL,10);
						if(i>16) moredrives=TRUE;
						if(i>0&&i<33)
						{
							strcpy(cfg_DriveText[i-1],word[2]);
							strcpy(cfg_Drive[i-1],word[3]);
						}
						else
							Error = TRUE;
					}
					else
						Error = TRUE;
				}

				}
				else
				{

				if(stricmp("VARIABLE",word[0])==0)
				{
					if(stricmp("FIRSTLEFT",word[1])==0)		{if(stricmp("Dirs",word[2])==0) cfg_FirstType[Left_Side]=MUIV_Dirlist_SortDirs_First; if(stricmp("Files",word[2])==0) cfg_FirstType[Left_Side]=MUIV_Dirlist_SortDirs_Last; if(stricmp("Mixed",word[2])==0) cfg_FirstType[Left_Side]=MUIV_Dirlist_SortDirs_Mix;}
					if(stricmp("FIRSTRIGHT",word[1])==0)	{if(stricmp("Dirs",word[2])==0) cfg_FirstType[Right_Side]=MUIV_Dirlist_SortDirs_First; if(stricmp("Files",word[2])==0) cfg_FirstType[Right_Side]=MUIV_Dirlist_SortDirs_Last; if(stricmp("Mixed",word[2])==0) cfg_FirstType[Right_Side]=MUIV_Dirlist_SortDirs_Mix;}
					if(stricmp("SORTLEFT",word[1])==0)		{if(stricmp("Name",word[2])==0) cfg_SortType[Left_Side]=MUIV_Dirlist_SortType_Name; if(stricmp("Date",word[2])==0) cfg_SortType[Left_Side]=MUIV_Dirlist_SortType_Date; if(stricmp("Size",word[2])==0) cfg_SortType[Left_Side]=MUIV_Dirlist_SortType_Size;}
					if(stricmp("SORTRIGHT",word[1])==0)		{if(stricmp("Name",word[2])==0) cfg_SortType[Right_Side]=MUIV_Dirlist_SortType_Name;	if(stricmp("Date",word[2])==0) cfg_SortType[Right_Side]=MUIV_Dirlist_SortType_Date;	if(stricmp("Size",word[2])==0) cfg_SortType[Right_Side]=MUIV_Dirlist_SortType_Size;}
					if(stricmp("ORDERLEFT",word[1])==0)		{if(stricmp("High",word[2])==0) cfg_SortHighLow[Left_Side]=FALSE; else cfg_SortHighLow[Left_Side]=TRUE;}
					if(stricmp("ORDERRIGHT",word[1])==0)	{if(stricmp("High",word[2])==0) cfg_SortHighLow[Right_Side]=FALSE; else cfg_SortHighLow[Right_Side]=TRUE;}
					if(stricmp("PAGES",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_PageType=TRUE; if(stricmp("OFF",word[2])==0) cfg_PageType=FALSE;}
					if(stricmp("MULTISELECT",word[1])==0)	{if(stricmp("ON",word[2])==0) cfg_MultiSelect=MUIV_Listview_MultiSelect_Always; if(stricmp("OFF",word[2])==0) cfg_MultiSelect=MUIV_Listview_MultiSelect_Shifted;}
				}

				if(stricmp("BUTTON",word[0])==0)
				{
					if((strlen(word[3])<81)&&(strlen(word[4])<81))
					{
						if(!buttonfound) for(i=0;i<120;i++) {strcpy(cfg_ButtonText[i],""); strcpy(cfg_ButtonCommand[i],""); cfg_ButtonType[i]=TRUE; global_MaximumBank=0;}
						buttonfound=TRUE;
						i=strtol(word[1],NULL,10);
						if(((i-1)/24)>global_MaximumBank) global_MaximumBank=((i-1)/24);
						if(i>0&&i<121)
						{
							if(stricmp("FUNCTION",word[2])==0) cfg_ButtonType[i-1]=TRUE;
							else cfg_ButtonType[i-1]=FALSE;
							strcpy(cfg_ButtonText[i-1],word[3]);
							strcpy(cfg_ButtonCommand[i-1],word[4]);
						}
						else
							Error = TRUE;
					}
					else
						Error = TRUE;
				}

				if(stricmp("HOTKEY",word[0])==0)
				{
					if((strlen(word[1])<41)&&(strlen(word[3])<81))
					{
						strcpy( cfg_HotKey[global_HotKeyNum], word[1] );
						if(stricmp("FUNCTION",word[2])==0) cfg_ButtonType[global_HotKeyNum+120]=TRUE;
						else cfg_ButtonType[global_HotKeyNum+120]=FALSE;
						strcpy( cfg_ButtonCommand[global_HotKeyNum+120],word[3]);
						global_HotKeyNum++;
					}
					else
						Error = TRUE;
				}

				if(stricmp("VARIABLE",word[0])==0)
				{
					if((stricmp("FORMATLEFT",word[1])==0)||(stricmp("FORMATRIGHT",word[1])==0))
					{
						if(strlen(word[2])<7)
						{
							strcpy(Format_String, "");
							for(i=0; i<strlen(word[2])+1; i++)
							{
								switch(word[2][i])
								{
									case 'N':
									case 'n':	strcat( Format_String, "COL=0" );
										break;

									case 'S':
									case 's':	strcat( Format_String, "COL=1 P=\33r" );
										break;

									case 'F':
									case 'f':	strcat( Format_String, "COL=4" );
										break;

									case 'D':
									case 'd':	strcat( Format_String, "COL=2" );
										break;

									case 'T':
									case 't':	strcat( Format_String, "COL=3" );
										break;

									case 'C':
									case 'c':	strcat( Format_String, "COL=5" );
										break;
								}
								if(i<strlen(word[2])-1) strcat( Format_String, ", " );
								else strcat( Format_String, "\0" );
							}
							if(stricmp("FORMATLEFT",word[1])==0)
								strcpy( cfg_Format[Left_Side], Format_String );
							if(stricmp("FORMATRIGHT",word[1])==0)
								strcpy( cfg_Format[Right_Side], Format_String );
						}
						else
							Error = TRUE;
					}

				}

				}

				if ( Error )
					printf( "ERROR: wrong configuration string in line %d\n", LineNumber );

				i = 0;
				num = 0;
			}
		}
		fclose(file);
	}

	if ( !FirstPass && !Error )
	{
		for(i=0;i<16;i++)  set(bt_Drive[i], MUIA_Text_Contents, cfg_DriveText[i]);
        if(!drivefound) set(Row[0], MUIA_ShowMe, FALSE);
		if(moredrives) for(i=16;i<32;i++) set(bt_Drive[i], MUIA_Text_Contents, cfg_DriveText[i]);
		else set(Row[1], MUIA_ShowMe, FALSE);
	}

	return( TRUE );
}

void SetMenu ( void )
{
	if ( cfg_CopyFlags )
		DoMethod( Menu, MUIM_SetUData, ID_CopyFlags, MUIA_Menuitem_Checked, TRUE);
	if ( cfg_CopyDate )
		DoMethod( Menu, MUIM_SetUData, ID_CopyDate, MUIA_Menuitem_Checked, TRUE);
	if ( cfg_CopyNote )
		DoMethod( Menu, MUIM_SetUData, ID_CopyNote, MUIA_Menuitem_Checked, TRUE);
	if ( cfg_CheckFit )
		DoMethod( Menu, MUIM_SetUData, ID_CheckSpace, MUIA_Menuitem_Checked, TRUE);
	if ( cfg_CopyVerify )
		DoMethod( Menu, MUIM_SetUData, ID_CopyVerify, MUIA_Menuitem_Checked, TRUE);

	if ( cfg_DelVerify )
		DoMethod( Menu, MUIM_SetUData, ID_Verify, MUIA_Menuitem_Checked, TRUE);
	if ( cfg_DelForce )
		DoMethod( Menu, MUIM_SetUData, ID_Force, MUIA_Menuitem_Checked, TRUE);

	if ( cfg_Overwrite == 0 )
		DoMethod( Menu, MUIM_SetUData, ID_OverYes, MUIA_Menuitem_Checked, TRUE);
	if ( cfg_Overwrite == 1 )
		DoMethod( Menu, MUIM_SetUData, ID_OverNo, MUIA_Menuitem_Checked, TRUE);
	if ( cfg_Overwrite == 2 )
		DoMethod( Menu, MUIM_SetUData, ID_OverVerify, MUIA_Menuitem_Checked, TRUE);
	if ( cfg_Overwrite == 3 )
		DoMethod( Menu, MUIM_SetUData, ID_OverOlder, MUIA_Menuitem_Checked, TRUE);
}
