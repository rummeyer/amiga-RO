/*
**
** Includes.h
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

#define Left_Side           0
#define Right_Side          1

/*
** Internal error codes
** Negative values distinguish these from AmigaDOS IoErr() codes (positive)
** Used by Error() in Misc.c to generate user-friendly error messages
*/
#define ERR_NO_MEMORY       -1
#define ERR_DATE_TO_STR     -2
#define ERR_STR_TO_DATE     -3
#define ERR_CMD_FAILED      -4
#define ERR_INFINITE_LOOP   -5
#define ERR_CANT_DELETE_SRC -6
#define ERR_FILE_EXISTS     -7
#define ERR_NO_XPK_LIB      -8
#define ERR_UNPACK_FAILED   -9
#define ERR_NO_DISK_SPACE   -10
#define ERR_EXPAND_PATH     -11
#define ERR_ABORTED         -12
#define ERR_VERIFY          -13
#define ERR_UNSUPPORTED     -14
#define ERR_SKIPPED         -20

enum Ids
{
	ID_Start,

	ID_About, ID_AboutMUI, ID_Iconify, ID_CopyFlags, ID_CopyDate, ID_CopyNote, ID_CheckSpace, ID_Verify, ID_Force, ID_OverYes, ID_OverNo,
	ID_OverVerify, ID_OverOlder, ID_MUIPrefs, ID_ROPrefs, ID_CopyVerify, ID_Message, ID_UpdateAll, ID_Spawn,

	ID_Menu_0,  ID_Menu_1,  ID_Menu_2,  ID_Menu_3,  ID_Menu_4,  ID_Menu_5,  ID_Menu_6,  ID_Menu_7,  ID_Menu_8,  ID_Menu_9,
	ID_Menu_10, ID_Menu_11, ID_Menu_12, ID_Menu_13, ID_Menu_14, ID_Menu_15, ID_Menu_16, ID_Menu_17, ID_Menu_18, ID_Menu_19,

	ID_Hot_0,  ID_Hot_1,  ID_Hot_2,  ID_Hot_3,  ID_Hot_4,  ID_Hot_5,  ID_Hot_6,  ID_Hot_7,  ID_Hot_8,  ID_Hot_9,

	ID_Key_0,  ID_Key_1,  ID_Key_2,  ID_Key_3,  ID_Key_4,  ID_Key_5,  ID_Key_6,  ID_Key_7,  ID_Key_8,  ID_Key_9,
	ID_Key_10,  ID_Key_11,  ID_Key_12,  ID_Key_13,  ID_Key_14,  ID_Key_15,  ID_Key_16,  ID_Key_17,  ID_Key_18,  ID_Key_19,
	ID_Key_20,  ID_Key_21,  ID_Key_22,  ID_Key_23,  ID_Key_24,  ID_Key_25,  ID_Key_26,  ID_Key_27,  ID_Key_28,  ID_Key_29,
	ID_Key_30,  ID_Key_31,  ID_Key_32,  ID_Key_33,  ID_Key_34,  ID_Key_35,  ID_Key_36,  ID_Key_37,  ID_Key_38,  ID_Key_39,
	ID_Key_40,  ID_Key_41,  ID_Key_42,  ID_Key_43,  ID_Key_44,  ID_Key_45,  ID_Key_46,  ID_Key_47,  ID_Key_48,  ID_Key_49,

	ID_DoubleClick, ID_VolumeLeft, ID_VolumeRight, ID_BufferLeft, ID_BufferRight, ID_UpdateTextLeft, ID_LeftActive,
	ID_UpdateTextRight, ID_StringEnterLeft, ID_StringEnterRight, ID_StatusLeft, ID_StatusRight, ID_RightActive,
	ID_StatusBar, ID_PageChange, ID_TildeCycle, ID_LeftChange, ID_RightChange, ID_OpenChange, ID_Sleep, ID_WakeUp,
	ID_StringOkay, ID_StringSkip, ID_StringAll, ID_StringCancel,

	ID_Stop,

	action_entries, addarc_entries,	copy_entries, copyas_entries, move_entries, moveas_entries, copydev_entries, delete_entries, command_entries,
	dup_entries, info_entries, listarc_entries, unarc_entries, comment_entries, rename_entries, setdate_entries, touch_entries, protect_entries,
	drop_entries
};
