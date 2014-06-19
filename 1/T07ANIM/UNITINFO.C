/* FILENAME: UNITINFO.C
 * PROGRAMMER: EF2
 * PURPOSE: Information unit handle module.
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* Information type of unit */
typedef struct tagef2UNIT_INFO
{
  EF2_UNIT_BASE_FIELDS; /* Base fields */
} ef2UNIT_INFO;

/* Init information unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID InfoUnitInit( ef2UNIT *Unit, ef2ANIM *Ani )
{
} /* End of 'EF2_AnimUnitInit' function */

/* Close information unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID InfoUnitClose( ef2UNIT *Unit, ef2ANIM *Ani )
{
} /* End of 'EF2_InfoUnitClose' function */

/* Response information unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID InfoUnitResponse( ef2UNIT *Unit, ef2ANIM *Ani )
{
  if (Ani->Keys['F'])
    EF2_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    EF2_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
  if (Ani->Keys[VK_ADD])
  {
    Ani->PosCam.X++;
    Ani->PosCam.Y++;
    Ani->PosCam.Z++;
  }
  if (Ani->Keys[VK_SUBTRACT])
  {
    Ani->PosCam.X--;
    Ani->PosCam.Y--;
    Ani->PosCam.Z--;
  }
} /* End of 'EF2_InfoUnitResponse' function */

/* Render information unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID InfoUnitRender( ef2UNIT *Unit, ef2ANIM *Ani )
{
  static CHAR Buf[1000];
  int i = 0;

} /* End of 'InfoUnitRender' function */

/* Create information unit of animation function.
 * ARGUMENTS:
 *   - size to structure of animation:
 *       INT Size;
 * RETURNS:
 *   (ef2UNIT *) pointer to unit.
 */
ef2UNIT * EF2_InfoUnitCreate( VOID )
{
  ef2UNIT_INFO *Unit;

  if ((Unit = (ef2UNIT_INFO *)EF2_AnimUnitCreate(sizeof(ef2UNIT_INFO))) == NULL)
    return NULL;
  /* Create fields by default */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Response = (VOID *)InfoUnitResponse;
  Unit->Render = (VOID *)InfoUnitRender;
  return (ef2UNIT *)Unit;
} /* End of 'EF2_AnimUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
