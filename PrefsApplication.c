#include "PrefsIncludes.h"
#include "PrefsApplication.h"

#define VERSION    "$VER: ROPrefs 0.81 (25.11.95)"

/*
**
** StartApplication()
**
*/

BOOL StartApplication ( void )
{

	app_RumorOpusPrefs = ApplicationObject,
		MUIA_Application_HelpFile, "RO.guide",
		MUIA_Application_Title         , "ROPrefs",
		MUIA_Application_Version       , VERSION,
		MUIA_Application_Copyright     , "Copyright © 1994, 1995 by Oliver Rummeyer",
		MUIA_Application_Author        , "Oliver Rummeyer",
		MUIA_Application_Description   , "MUI-Based FileManager Prefs",
		MUIA_Application_Base          , "ROPREFS",

		SubWindow,
			wi_Main = WindowObject,
			MUIA_Window_Title, "RO Prefs 0.81 (25.11.95)",
			MUIA_Window_ID, 10,
			MUIA_Window_Menustrip, Menu = MUI_MakeObject( MUIO_MenustripNM, MenuData, 0),
			MUIA_Window_NeedsMouseObject, TRUE,
			MUIA_Window_AppWindow, FALSE,
			MUIA_HelpNode, "Configuration",
			WindowContents, VGroup,

				Child, pg_Page = RegisterGroup( Pages ), MUIA_Register_Frame, TRUE,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, ColGroup(2),
							Child, KeyLabel1("CheckDupes",'u'), Child, cm_CheckDupes = KeyCheckMark(TRUE,'u'),
							Child, KeyLabel1("CreateIcons",'i'), Child, cm_CreateIcons = KeyCheckMark(TRUE,'i'),
							Child, KeyLabel1("FlushMemory",'f'), Child, cm_FlushMemory = KeyCheckMark(FALSE,'f'),
							Child, KeyLabel1("FollowScroll",'r'), Child, cm_FollowScroll = KeyCheckMark(TRUE,'r'),
							Child, KeyLabel1("PathExpand",'x'), Child, cm_PathExpand = KeyCheckMark(TRUE,'x'),
							Child, KeyLabel1("QuitVerify",'q'), Child, cm_QuitVerify = KeyCheckMark(TRUE,'q'),
							Child, KeyLabel1("Symmetrical",'m'), Child, cm_Symmetrical = KeyCheckMark(FALSE,'m'),
							End,
						Child, ColGroup(2),
							Child, KeyLabel1("ShowDay",'w' ), Child, cm_ShowDay = KeyCheckMark(TRUE,'w'),
							Child, KeyLabel1("ShowDate",'o'), Child, cm_ShowDate = KeyCheckMark(TRUE,'o'),
							Child, KeyLabel1("ShowTime",'t'), Child, cm_ShowTime = KeyCheckMark(TRUE,'t'),
							Child, KeyLabel1("ShowSeconds",'n'), Child, cm_ShowSeconds = KeyCheckMark(FALSE,'n'),
							Child, KeyLabel1("ShowMemory",'y'), Child, cm_ShowMemory = KeyCheckMark(TRUE,'y'),
							Child, KeyLabel1("DelVerify",'d'), Child, cm_DelVerify = KeyCheckMark(TRUE,'d'),
							Child, KeyLabel1("DelForce",'l'), Child, cm_DelForce = KeyCheckMark(FALSE,'l'),
							End,
						Child, ColGroup(2),
							Child, KeyLabel1("DelStat",'a'), Child, cm_DelStat = KeyCheckMark(TRUE,'a'),
							Child, KeyLabel1("CheckFit",'h'), Child, cm_CheckFit = KeyCheckMark(TRUE,'h'),
							Child, KeyLabel1("CopyDate",'c' ), Child, cm_CopyDate = KeyCheckMark(TRUE,'c'),
							Child, KeyLabel1("CopyFlags",'g'), Child, cm_CopyFlags = KeyCheckMark(TRUE,'g'),
							Child, KeyLabel1("CopyNote",'p'), Child, cm_CopyNote = KeyCheckMark(TRUE,'p'),
							Child, KeyLabel1("CopyStat",'s'), Child, cm_CopyStat = KeyCheckMark(TRUE,'s'),
							Child, KeyLabel1("CopyVerify",'v'), Child, cm_CopyVerify = KeyCheckMark(FALSE,'v'),
							Child, VSpace(0), Child, VSpace(0),
							Child, VSpace(0), Child, VSpace(0),
							End,
						End,

					Child, HGroup,
						Child, ColGroup(2),
							Child, KeyLabel1("ClockPri", 'i'), Child, st_ClockPri = KeyString( "", 5, 'i' ),
							Child, KeyLabel1("ClockRefresh", 'r' ), Child, st_Refresh = KeyString( "", 10, 'r' ),
							Child, KeyLabel1("CopyBuffer" , 'b' ), Child, st_Buffer =  KeyString( "", 41, 'b' ),
							Child, KeyLabel1("DelDelay", 'e' ), Child, st_Delay = KeyString( "", 41, 'e' ),
							Child, KeyLabel1("History", 'y' ), Child, st_History = KeyString( "", 41, 'y' ),
							Child, KeyLabel1("Output", 'p'), Child, st_Output = KeyString( "", 81, 'p' ),
							Child, KeyLabel1("TempDir",'t'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_TempDir = KeyString( "", 81, 't' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							End,
						Child, HSpace(1),
						Child, ColGroup(2),
							Child, KeyLabel1("AppWindow",'a'), Child, cy_AppWindow = KeyCycle(CYA_AppWindow,'a'),
							Child, KeyLabel1("Completed",'c'), Child, cy_Completed = KeyCycle(CYA_Completed,'c'),
							Child, KeyLabel1("DriveButtons",'d'), Child, cy_DriveMode = KeyCycle(CYA_Mode,'d'),
							Child, KeyLabel1("HotDirs",'h'), Child, cy_HotMode = KeyCycle(CYA_Mode,'h'),
							Child, KeyLabel1("MiddleMouse",'m'), Child, cy_MiddleMouse = KeyCycle(CYA_MiddleMouse,'m'),
							Child, KeyLabel1("Overwrite",'o'), Child, cy_Overwrite = KeyCycle(CYA_Overwrite,'o'),
							Child, VSpace(0), Child, VSpace(0),
							Child, VSpace(0), Child, VSpace(0),
							End,
						End,

					Child, HGroup,
						Child, ColGroup(2), GroupFrameT("Left"),
							Child, KeyLabel1("Page",'p'), Child, cy_PageLeft = KeyCycle(CYA_Pages,'p'),
							Child, KeyLabel1("First",'f'), Child, cy_FirstLeft = KeyCycle(CYA_First,'f'),
							Child, KeyLabel1("Sort",'s'), Child, cy_SortLeft = KeyCycle(CYA_Sort,'s'),
							Child, KeyLabel1("Load",'l'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_LoadLeft = KeyString( "", 81, 'l' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("Format",'m'),
							Child, pp_FormatLeft = PopobjectObject,
								MUIA_Popstring_String, st_FormatLeft = KeyString( "", 7, 'm' ),
								MUIA_Popstring_Button, bt_FormatLeft = PopButton(MUII_PopUp),
								MUIA_Popobject_StrObjHook, &StrObjHook,
								MUIA_Popobject_ObjStrHook, &ObjStrHook2,
								MUIA_Popobject_WindowHook, &WindowHook,
								MUIA_Popobject_Object, pp_FormatLeftList = ListviewObject,
									MUIA_Listview_List, ListObject,
										InputListFrame,
										MUIA_List_SourceArray, PopFormat,
										End,
									End,
								End,
							End,
						Child, ColGroup(2), GroupFrameT("Right"),
							Child, KeyLabel1("Page",'a'), Child, cy_PageRight = KeyCycle(CYA_Pages,'a'),
							Child, KeyLabel1("First",'i'), Child, cy_FirstRight = KeyCycle(CYA_First,'i'),
							Child, KeyLabel1("Sort",'o'), Child, cy_SortRight = KeyCycle(CYA_Sort,'o'),
							Child, KeyLabel1("Load",'d'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_LoadRight = KeyString( "", 81, 'd' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("Format",'r'),
							Child, pp_FormatRight = PopobjectObject,
								MUIA_Popstring_String, st_FormatRight = KeyString( "", 7, 'r' ),
								MUIA_Popstring_Button, bt_FormatRight = PopButton(MUII_PopUp),
								MUIA_Popobject_StrObjHook, &StrObjHook,
								MUIA_Popobject_ObjStrHook, &ObjStrHook2,
								MUIA_Popobject_WindowHook, &WindowHook,
								MUIA_Popobject_Object, pp_FormatRightList = ListviewObject,
									MUIA_Listview_List, ListObject,
										InputListFrame,
										MUIA_List_SourceArray, PopFormat,
										End,
									End,
								End,
							End,
						End,

					Child, HGroup,
						Child, ColGroup(2),
							Child, KeyLabel1("F1",'1'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[0] = KeyString( "", 81, '1' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("F2",'2'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[1] = KeyString( "", 81, '2' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("F3",'3'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[2] = KeyString( "", 81, '3' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("F4",'4'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[3] = KeyString( "", 81, '4' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("F5",'5'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[4] = KeyString( "", 81, '5' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							End,
						Child, HSpace(1),
						Child, ColGroup(2),
							Child, KeyLabel1("F6",'6'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[5] = KeyString( "", 81, '6' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("F7",'7'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[6] = KeyString( "", 81, '7' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("F8",'8'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[7] = KeyString( "", 81, '8' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("F9",'9'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[8] = KeyString( "", 81, '9' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							Child, KeyLabel1("F10",'0'),
							Child, PopaslObject,
								MUIA_Popstring_String, st_HotDir[9] = KeyString( "", 81, '0' ),
								MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
								ASLFR_TitleText  , "Select a drawer...",
								ASLFR_DrawersOnly, TRUE,
								End,
							End,
						End,

					Child, HGroup,
						Child, VGroup,
							Child, lv_Filetypes = ListviewObject,
								MUIA_Font, MUIV_Font_Fixed,
								MUIA_Listview_Input, TRUE,
								MUIA_Listview_List,
								ListObject,
									InputListFrame,
									MUIA_List_ConstructHook, MUIV_List_ConstructHook_String,
									MUIA_List_DestructHook, MUIV_List_DestructHook_String,
									End,
								End,
							Child, st_Filetypes = StringObject,
								StringFrame,
								MUIA_String_AttachedList, lv_Filetypes,
								MUIA_String_MaxLen, 81,
								End,
							Child, HGroup, MUIA_Group_SameSize, TRUE,
								Child, bt_AddFiletype = KeyButton("Add",'a'),
								Child, bt_DelFiletype = KeyButton("Remove",'r'),
								End,
							End,
						Child, HSpace(1),
						Child, VGroup, MUIA_Weight, 50,
							Child, ColGroup(2),
								Child, KeyLabel1("Type",'t'), Child, cy_Filetype = KeyCycle(CYA_Type,'t'),
								Child, KeyLabel1("Recognize", 'e' ), Child, st_Recog = KeyString( "", 81, 'e' ),
								Child, KeyLabel1("Match", 'm' ), Child, st_Pattern = KeyString( "", 81, 'm' ),
								Child, KeyLabel1("1st Command",'1'),
								Child, pp_CommandA = PopobjectObject,
									MUIA_Popstring_String, st_CommandA = KeyString( "", 81, '1' ),
									MUIA_Popstring_Button, PopButton(MUII_PopUp),
									MUIA_Popobject_StrObjHook, &StrObjHook,
									MUIA_Popobject_ObjStrHook, &ObjStrHook3,
									MUIA_Popobject_WindowHook, &WindowHook,
									MUIA_Popobject_Object, pp_ComAList = ListviewObject,
										MUIA_Listview_List, ListObject,
											InputListFrame,
											MUIA_List_SourceArray, PopVars,
											End,
										End,
									End,
								Child, KeyLabel1("2nd Command",'2'),
								Child, pp_CommandB = PopobjectObject,
									MUIA_Popstring_String, st_CommandB = KeyString( "", 81, '2' ),
									MUIA_Popstring_Button, PopButton(MUII_PopUp),
									MUIA_Popobject_StrObjHook, &StrObjHook,
									MUIA_Popobject_ObjStrHook, &ObjStrHook3,
									MUIA_Popobject_WindowHook, &WindowHook,
									MUIA_Popobject_Object, pp_ComBList = ListviewObject,
										MUIA_Listview_List, ListObject,
											InputListFrame,
											MUIA_List_SourceArray, PopVars,
											End,
										End,
									End,
								End,
							Child, VSpace(0),
							End,
						End,

					Child, HGroup,
						Child, VGroup,
							Child, lv_Input = ListviewObject,
								MUIA_Font, MUIV_Font_Fixed,
								MUIA_Listview_Input, TRUE,
								MUIA_Listview_List,
								ListObject,
									InputListFrame,
									MUIA_List_ConstructHook, MUIV_List_ConstructHook_String,
									MUIA_List_DestructHook, MUIV_List_DestructHook_String,
									End,
								End,
							Child, st_Input = StringObject,
								StringFrame,
								MUIA_String_MaxLen, 81,
								MUIA_String_AttachedList, lv_Input,
								End,
							Child, HGroup, MUIA_Group_SameSize, TRUE,
								Child, bt_AddEvent = KeyButton("Add",'a'),
								Child, bt_DelEvent = KeyButton("Remove",'r'),
								End,
							End,
						Child, HSpace(1),
						Child, VGroup, MUIA_Weight, 50,
							Child, ColGroup(2),
								Child, KeyLabel1("Event",'e'), Child, cy_Event = KeyCycle(CYA_Event,'e'),
								Child, KeyLabel1("Type",'t'), Child, cy_Type = KeyCycle(CYA_TypeBtn,'t'),
								Child, KeyLabel1("Function",'f'),
								Child, pp_Function = PopobjectObject,
									MUIA_Popstring_String, st_Function = KeyString( "", 81, 'c' ),
									MUIA_Popstring_Button, PopButton(MUII_PopUp),
									MUIA_Popobject_StrObjHook, &StrObjHook,
									MUIA_Popobject_ObjStrHook, &ObjStrHook,
									MUIA_Popobject_WindowHook, &WindowHook,
									MUIA_Popobject_Object, pp_FunList = ListviewObject,
										MUIA_Listview_List, ListObject,
											InputListFrame,
											MUIA_List_SourceArray, PopNames,
											End,
										End,
									End,
								Child, KeyLabel1("Command",'c'),
								Child, pp_Command = PopobjectObject,
									MUIA_Popstring_String, st_Command = KeyString( "", 81, 'c' ),
									MUIA_Popstring_Button, PopButton(MUII_PopUp),
									MUIA_Popobject_StrObjHook, &StrObjHook,
									MUIA_Popobject_ObjStrHook, &ObjStrHook3,
									MUIA_Popobject_WindowHook, &WindowHook,
									MUIA_Popobject_Object, pp_ComList = ListviewObject,
										MUIA_Listview_List, ListObject,
											InputListFrame,
											MUIA_List_SourceArray, PopVars,
											End,
										End,
									End,
								Child, KeyLabel1("Directory",'y'),
								Child, pp_Directory = PopaslObject,
									MUIA_Popstring_String, st_Directory = KeyString( "", 81, 'y' ),
									MUIA_Popstring_Button, PopButton(MUII_PopDrawer),
									ASLFR_TitleText  , "Select a drawer...",
									ASLFR_DrawersOnly, TRUE,
									End,
								End,
							Child, VSpace(0),
							End,
						End,
					End,

				Child, HGroup, MUIA_Group_SameSize, TRUE,
					Child, bt_Save = SimpleButton("Save"),
					Child, HSpace(0),
					Child, bt_Use = SimpleButton("Use"),
					Child, HSpace(0),
					Child, bt_Cancel = SimpleButton("Cancel"),
					End,
				End,
			End,
		End;

	if ( !app_RumorOpusPrefs )
		return( FALSE );
	else
		return( TRUE );
}

/*
**
** StopApplication()
**
*/

BOOL StopApplication ( void )
{
	if ( app_RumorOpusPrefs )
		MUI_DisposeObject( app_RumorOpusPrefs );

	return( TRUE );
}

/*
**
** Methods()
**
*/

void Methods ( void )
{
	/*** Notifiy for input events page ***/
	DoMethod( lv_Input,     MUIM_Notify, MUIA_List_Active, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_EventSelect );
	DoMethod( lv_Input,     MUIM_Notify, MUIA_List_Active, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_EventtypeChange );
	DoMethod( cy_Event,     MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_EventtypeChange );
	DoMethod( cy_Type,      MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_CommandtypeChange );
	DoMethod( st_Directory, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_DirEnter );
	DoMethod( st_Function,  MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_FunctionEnter );
	DoMethod( st_Command,   MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_CommandEnter );
	DoMethod( st_Input,     MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_EnterEvent );
	DoMethod( bt_AddEvent,  MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_AddEvent );
	DoMethod( bt_DelEvent,  MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_DelEvent );
	DoMethod( pp_ComList,   MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, pp_Command, 2, MUIM_Popstring_Close, TRUE);
	DoMethod( pp_FunList,   MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, pp_Function, 2, MUIM_Popstring_Close, TRUE);

	/*** Notifiy for filetypes page ***/
	DoMethod( lv_Filetypes,   MUIM_Notify, MUIA_List_Active, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_FiletypeSelect );
	DoMethod( cy_Filetype,    MUIM_Notify, MUIA_Disabled, TRUE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_FiletypeDisabled );
	DoMethod( cy_Filetype,    MUIM_Notify, MUIA_Disabled, FALSE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_FiletypeEnabled );
	DoMethod( bt_AddFiletype, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_AddFiletype );
	DoMethod( bt_DelFiletype, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_DelFiletype );
	DoMethod( st_Filetypes,   MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_FiletypeEnter );
	DoMethod( st_Recog,       MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_RecogEnter );
	DoMethod( st_Pattern,     MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_PatternEnter );
	DoMethod( st_CommandA,    MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_CommandAEnter );
	DoMethod( st_CommandB,    MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_CommandBEnter );
	DoMethod( cy_Filetype,    MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_FiletypeChange );
	DoMethod( pp_ComAList,    MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, pp_CommandA, 2, MUIM_Popstring_Close, TRUE);
	DoMethod( pp_ComBList,    MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, pp_CommandB, 2, MUIM_Popstring_Close, TRUE);

	/*** Notifiy for main window ***/
	DoMethod( wi_Main,   MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit );
	DoMethod( bt_Cancel, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit );
	DoMethod( bt_Save,   MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_Save_Btn );
	DoMethod( bt_Use,    MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_Use_Btn );
	DoMethod( pp_FormatLeftList,      MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, pp_FormatLeft, 2, MUIM_Popstring_Close, TRUE);
	DoMethod( pp_FormatRightList,      MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, pp_FormatRight, 2, MUIM_Popstring_Close, TRUE);
	DoMethod( st_FormatLeft, MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_FormatLeftEnter );
	DoMethod( st_FormatRight, MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime, app_RumorOpusPrefs, 2, MUIM_Application_ReturnID, ID_FormatRightEnter );
	DoMethod( cm_ShowSeconds, MUIM_Notify, MUIA_Selected, TRUE, cm_ShowTime, 3, MUIM_Set, MUIA_Selected, TRUE );
	DoMethod( cm_ShowTime, MUIM_Notify, MUIA_Selected, FALSE, cm_ShowSeconds, 3, MUIM_Set, MUIA_Selected, FALSE );

	DoMethod(wi_Main, MUIM_Window_SetCycleChain, st_ClockPri, st_Refresh, st_Buffer, st_Delay, st_History, st_Output, st_TempDir,
	cy_AppWindow, cy_Completed, cy_DriveMode, cy_HotMode, cy_MiddleMouse, cy_Overwrite, cy_PageLeft, cy_FirstLeft, cy_SortLeft,
	st_LoadLeft, st_FormatLeft, cy_PageRight, cy_FirstRight, cy_SortRight, st_LoadRight, st_FormatRight, st_HotDir[0], st_HotDir[1],
	st_HotDir[2], st_HotDir[3], st_HotDir[4], st_HotDir[5], st_HotDir[6], st_HotDir[7], st_HotDir[8], st_HotDir[9],
	st_Input, bt_AddEvent, bt_DelEvent, cy_Event, cy_Type, st_Command, st_Directory, st_Filetypes, bt_AddFiletype,
	bt_DelFiletype, cy_Filetype, st_Recog, st_Pattern, st_CommandA, st_CommandB, bt_Save, bt_Use, bt_Cancel, NULL);
}
