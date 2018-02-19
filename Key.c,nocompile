#include "Includes.h"
#include "Key.h"

/*
**
** CheckKey()
**
*/

void CheckKey( void )
{

 struct KeyData {
  char username[41];
  char userstreet[41];
  char serialadder;
  char usercity[41];
  char userserial[5];
  char cityadder;
  char garbage[28];
  char usernamechk[41];
  char nameadder;
  char usercitychk[41];
  char userstreetchk[41];
  char streetadder;
  char userserialchk[5];
  ULONG checkall;
 };

 struct KeyData kd[1];
 int fh;
 register int i;
 char bak,bak1;
 ULONG check=0;
 char keys[256];
 char usercity[41];
 char userstreet[41];

 fh=open("PROGDIR:RO.key",O_RDONLY);
 if(fh==-1) fh=open("L:RO.key",O_RDONLY);
 if(fh==-1) fh=open("S:RO.key",O_RDONLY);
 if(fh==-1)
 {
  GetVar( "KEYPATH", keys, 245, 0 );
  AddPart( keys, "RO.key", sizeof( keys ) + 10 );
  fh=open(keys,O_RDONLY);
 }

 if(fh!=-1)
 {
  read(fh,kd,sizeof(kd));
  strcpy(global_Username,kd[0].username);
  strcpy(userstreet,kd[0].userstreet);
  strcpy(usercity,kd[0].usercity);
  strcpy(global_Userserial,kd[0].userserial);

  if((kd[0].nameadder==17)&&(kd[0].streetadder==4)&&(kd[0].cityadder==23)&&(kd[0].serialadder==9))
    global_KeyFile = TRUE;
  else
    global_KeyFile = FALSE;

  for(i=0;i<strlen(global_Username);i++)
  {
    check=check+global_Username[i];
    bak=global_Username[i]^(255-i-kd[0].nameadder);
    bak1=kd[0].usernamechk[i]^(255-i-kd[0].nameadder);
    if (bak!=kd[0].usernamechk[i])
      global_KeyFile=FALSE;
  }

  for(i=0;i<strlen(userstreet);i++)
  {
    check=check+userstreet[i];
    bak=userstreet[i]^(255-i-kd[0].streetadder);
    bak1=kd[0].userstreetchk[i]^(255-i-kd[0].streetadder);
    if (bak!=kd[0].userstreetchk[i])
      global_KeyFile=FALSE;
  }

  for(i=0;i<strlen(usercity);i++)
  {
    check=check+usercity[i];
    bak=usercity[i]^(255-i-kd[0].cityadder);
    bak1=kd[0].usercitychk[i]^(255-i-kd[0].cityadder);
    if (bak!=kd[0].usercitychk[i])
      global_KeyFile=FALSE;
  }

  for(i=0;i<strlen(global_Userserial);i++)
  {
    check=check+global_Userserial[i];
    bak=global_Userserial[i]^(255-i-kd[0].serialadder);
    bak1=kd[0].userserialchk[i]^(255-i-kd[0].serialadder);
    if (bak!=kd[0].userserialchk[i])
      global_KeyFile=FALSE;
  }

  if(kd[0].checkall!=check) global_KeyFile=FALSE;

  close(fh);
 }
 else global_KeyFile=FALSE;

}
