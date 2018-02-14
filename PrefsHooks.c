#include "PrefsIncludes.h"
#include "PrefsHooks.h"

SAVEDS ASM LONG StrObjFunc(REG(a2) Object *pop,REG(a1) Object *str)
{
	char *x,*s;
	int i;

	get(str,MUIA_String_Contents,&s);

	for (i=0;;i++)
	{
		DoMethod(pop,MUIM_List_GetEntry,i,&x);
		if (!x)
		{
			set(pop,MUIA_List_Active,MUIV_List_Active_Off);
			break;
		}
		else if (!stricmp(x,s))
		{
			set(pop,MUIA_List_Active,i);
			break;
		}
	}
	return(TRUE);
}


SAVEDS ASM VOID ObjStrFunc(REG(a2) Object *pop,REG(a1) Object *str)
{
	char *x;
	DoMethod(pop,MUIM_List_GetEntry,MUIV_List_GetEntry_Active,&x);
	set(str,MUIA_String_Contents,x);
	set(str,MUIA_String_Acknowledge,TRUE);
}


SAVEDS ASM VOID WindowFunc(REG(a2) Object *pop,REG(a1) Object *win)
{
	set(win,MUIA_Window_DefaultObject,pop);
}

SAVEDS ASM VOID ObjStrFunc2(REG(a2) Object *pop,REG(a1) Object *str)
{
	char *a,*b,c[7];
	DoMethod(pop,MUIM_List_GetEntry,MUIV_List_GetEntry_Active,&a);
	get(str,MUIA_String_Contents,&b);
	strcpy(c,b);
	strncat(c,a,1);
	set(str,MUIA_String_Contents,c);
	set(str,MUIA_String_Acknowledge,TRUE);
}

SAVEDS ASM VOID ObjStrFunc3(REG(a2) Object *pop,REG(a1) Object *str)
{
	char *a,*b,c[81];
	DoMethod(pop,MUIM_List_GetEntry,MUIV_List_GetEntry_Active,&a);
	get(str,MUIA_String_Contents,&b);
	strcpy(c,b);
	strcat(c," ");
	strncat(c,a,2);
	set(str,MUIA_String_Contents,c);
	set(str,MUIA_String_Acknowledge,TRUE);
}
