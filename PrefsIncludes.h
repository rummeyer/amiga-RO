/*
**
** PrefsIncludes.h
**
*/

#include <libraries/mui.h>
#include <libraries/xpk.h>
#include <dos.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <graphics/gfxmacros.h>
#include <workbench/workbench.h>
#include <exec/memory.h>
#include <exec/types.h>
#include <exec/execbase.h>
#include <clib/alib_protos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/icon_protos.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/utility_protos.h>
#include <clib/asl_protos.h>
#include <clib/muimaster_protos.h>
#include <proto/locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <pragmas/exec_sysbase_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/icon_pragmas.h>
#include <pragmas/graphics_pragmas.h>
#include <pragmas/intuition_pragmas.h>
#include <pragmas/gadtools_pragmas.h>
#include <pragmas/utility_pragmas.h>
#include <pragmas/asl_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include "WBPath.h"

extern struct Library * SysBase;
extern struct Library * LocaleBase;
extern struct Library * IntuitionBase;
extern struct Library * UtilityBase;
extern struct Library * GfxBase;
extern struct Library * DOSBase;
extern struct Library * IconBase;
extern struct Library * XpkBase;
extern struct Library * MUIMasterBase;

#define ASM    __asm
#define SAVEDS __saveds
#define REG(x) register __ ## x

enum Ids
{
	ID_dummy,

	ID_Save_Btn, ID_Use_Btn, ID_MenuOpen, ID_MenuSaveAs, ID_MenuAbout, ID_MenuReset, ID_MenuLast, ID_MenuRestore,
	ID_EventSelect, ID_DirEnter, ID_CommandEnter, ID_CommandtypeChange, ID_EventtypeChange, ID_EnterEvent,
	ID_AddEvent, ID_DelEvent, ID_FiletypeSelect, ID_FiletypeDisabled, ID_FiletypeEnabled, ID_AddFiletype,
	ID_DelFiletype, ID_FiletypeEnter, ID_RecogEnter, ID_PatternEnter, ID_CommandAEnter, ID_CommandBEnter,
	ID_FiletypeChange, ID_FormatRightEnter, ID_FormatLeftEnter, ID_FunctionEnter, ID_MenuIconify, ID_MUIPrefs
};
