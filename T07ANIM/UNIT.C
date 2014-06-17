/* FILENAME: UNIT.C
 * PROGRAMMER: SS3
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

static VOID SS3_AnimUnitInit( ss3UNIT *Unit, ss3ANIM *Ani )
{
}

static VOID SS3_AnimUnitClose( ss3UNIT *Unit, ss3ANIM *Ani )
{
}

static VOID SS3_AnimUnitResponse( ss3UNIT *Unit, ss3ANIM *Ani )
{
}

static VOID SS3_AnimUnitRender( ss3UNIT *Unit, ss3ANIM *Ani )
{
}

ss3UNIT * SS3_AnimUnitCreate( INT Size )
{
  ss3UNIT *Unit;

  if (Size < sizeof(ss3UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);

  Unit->Size = Size;
  Unit->Init = SS3_AnimUnitInit;
  Unit->Close = SS3_AnimUnitClose;
  Unit->Response = SS3_AnimUnitResponse;
  Unit->Render = SS3_AnimUnitRender;
  return Unit;
}