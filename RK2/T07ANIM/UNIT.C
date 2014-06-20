/* FILENAME: UNIT.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* Default Unit init function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID RK2_AnimUnitInit( rk2UNIT *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_AnimUnitInit' function */

/* Default unit destructor function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID RK2_AnimUnitClose( rk2UNIT *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_AnimUnitClose' function */

/* Default unit response function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID RK2_AnimUnitResponse( rk2UNIT *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_AnimUnitResponse' function */

/* Default unit render function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
static VOID RK2_AnimUnitRender( rk2UNIT *Unit, rk2ANIM *Ani )
{
} /* End of 'RK2_AnimUnitRender' function */

/* Animation default unit create function.
 * ARGUMENTS:
 *   - Self pointer:
 *       rk2UNIT *Unit;
 *   - Animation context pointer:
 *       rk2ANIM *Ani;
 * RETURNS: None.
 */
rk2UNIT * RK2_AnimUnitCreate( INT Size )
{
  rk2UNIT *Unit;

  if (Size < sizeof(rk2UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* Base fields */
  Unit->Size = Size;
  Unit->Init = RK2_AnimUnitInit;
  Unit->Close = RK2_AnimUnitClose;
  Unit->Response = RK2_AnimUnitResponse;
  Unit->Render = RK2_AnimUnitRender;
  return Unit;
} /* End of 'RK2_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

