/* FILENAME: UNIT.C
 * PROGRAMMER: EF2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* Default Init unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID EF2_AnimUnitInit( ef2UNIT *Unit, ef2ANIM *Ani )
{
} /* End of 'EF2_AnimUnitInit' function */

/* Close unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID EF2_AnimUnitClose( ef2UNIT *Unit, ef2ANIM *Ani )
{
} /* End of 'EF2_AnimUnitClose' function */

/* Response unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID EF2_AnimUnitResponse( ef2UNIT *Unit, ef2ANIM *Ani )
{
} /* End of 'EF2_AnimUnitResponse' function */

/* Render unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID EF2_AnimUnitRender( ef2UNIT *Unit, ef2ANIM *Ani )
{
} /* End of 'EF2_AnimUnitRender' function */

/* Create unit of animation function.
 * ARGUMENTS:
 *   - size to structure of animation:
 *       INT Size;
 * RETURNS:
 *   (ef2UNIT *) pointer to unit.
 */
ef2UNIT * EF2_AnimUnitCreate( INT Size )
{
  ef2UNIT *Unit;

  if (Size < sizeof(ef2UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* Create fields by default */
  Unit->Size = Size;
  Unit->Init = EF2_AnimUnitInit;
  Unit->Close = EF2_AnimUnitClose;
  Unit->Response = EF2_AnimUnitResponse;
  Unit->Render = EF2_AnimUnitRender;
  return Unit;
} /* End of 'EF2_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

