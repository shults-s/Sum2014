/* FILENAME: RENDER.C
 * PROGRAMMER: RK2
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 12.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "mth.h"

/* Setting cameras properties 
 * ARGUMENTS:
 *   - Vector camera new properties:
 *     rk2VEC Loc, At, Up;
 * RETURNS: None.
 */
VOID RK2_RndCameraSet( rk2CAMERA *Cam, rk2VEC Loc, rk2VEC At, rk2VEC Up )
{
  Cam->Loc = Loc;
  Cam->At = At;
  Cam->Up = Up;
} /* End of 'RK2_RndCameraSet' funciton */

/* Cameras getting matrix function.
* ARGUMENTS: None.
* RETURNS: 
*   (rk2MATR4x4) Camera matrix.
*/
static rk2MATR4x4 RK2_RndCameraGetMatrix( rk2CAMERA *Cam )
{
  rk2MATR4x4 RetMatr = 
  {
    {
      { Cam->Dir.X  , Cam->Dir.Y  , Cam->Dir.Z  , 0 },
      { Cam->Up.X   , Cam->Up.Y   , Cam->Up.Z   , 0 },
      { Cam->Right.X, Cam->Right.Y, Cam->Right.Z, 0 },
      {                  0,                  0,                 0 , 1 }
    }
  };
  return RetMatr;
} /* End of 'RK2_RndCameraGetMatrix' funciton */

/* Cameras normalizing vectors function.
* ARGUMENTS: None.
* RETURNS: None.
*/
static VOID RK2_RndCameraNormalize( rk2CAMERA *Cam )
{
  Cam->At = VecNormalize(Cam->At);
  Cam->Dir = VecNormalize(Cam->Dir);
  Cam->Up = VecNormalize(Cam->Up);
  Cam->Right = VecNormalize(Cam->Right);
} /* End of 'RK2_RndCameraGetMatrix' funciton */

/* Camera move by Dir-axes properties 
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID RK2_RndCameraMoveDir( rk2CAMERA *Cam, DBL Coef )
{
  Cam->Dir = VecNormalize(VecSubVec(Cam->At, Cam->Loc));
  Cam->Loc = VecSumVec(Cam->Loc, VecMultNum(Cam->Dir, Coef));
  Cam->At = VecSumVec(Cam->At, VecMultNum(Cam->Dir, Coef));
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Camera move by Right-Axes function.
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID RK2_RndCameraMoveRight( rk2CAMERA *Cam, DBL Coef )
{
  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(Cam->Dir, Cam->Up);
  Cam->Loc = VecSumVec(Cam->Loc, VecMultNum(VecNormalize(Cam->Right), Coef));
  Cam->At = VecSumVec(Cam->At, VecMultNum(VecNormalize(Cam->Right), Coef));
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Camera move by Up-Axes function.
 * ARGUMENTS:
 *   - Distance to move:
 *       DBL Coef;  
 * RETURNS: None.
 */
VOID RK2_RndCameraMoveUp( rk2CAMERA *Cam, DBL Coef )
{
  Cam->Loc = VecSumVec(Cam->Loc, VecMultNum(VecNormalize(Cam->Up), Coef));
  Cam->At = VecSumVec(Cam->At, VecMultNum(VecNormalize(Cam->Up), Coef));
}  /* End of 'RK2_RndCameraMoveByDir' funciton */

/* Camera rotation by camera-axes-Up function.
 * ARGUMENTS:
 *   - Angle:
 *       DBL Angle;
 * RETURNS: None.
 */
VOID RK2_RndCameraRotateUp( rk2CAMERA *Cam, DBL Angle )
{
  rk2MATR4x4 RotMat;

  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(VecNormalize(Cam->Dir), Cam->Up);

  RK2_RndCameraNormalize(Cam);

  RotMat = RK2_RndCameraGetMatrix(Cam);
  RotMat = MatrMultMatr(MatrRotateY(MatrDefault(), Angle), RotMat);

  RK2_RndCameraSet(Cam, Cam->Loc,                                                                  /* Location */
                   VecSumVec(Cam->Loc, 
                             VecSet(RotMat.A[0][0], RotMat.A[0][1], RotMat.A[0][2])),              /* At */
                   Cam->Up);                                                                       /* Up */
}  /* End of 'RK2_RndCameraRotateUp' funciton */

/* Camera rotation by camera-axes-right function.
* ARGUMENTS:
*   - Angle:
*       DBL Angle;
* RETURNS: None.
*/
VOID RK2_RndCameraRotateRight( rk2CAMERA *Cam, DBL Angle )
{
  rk2MATR4x4 RotMat;
  /*
  if (Angle == 0)
    return;
  */
  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(VecNormalize(Cam->Dir), Cam->Up);

  RK2_RndCameraNormalize(Cam);

  RotMat = RK2_RndCameraGetMatrix(Cam);
  RotMat = MatrMultMatr(MatrRotateZ(MatrDefault(), Angle), RotMat);

  RK2_RndCameraSet(Cam, Cam->Loc,                                               /* Location */
    VecSumVec(Cam->Loc,
    VecSet(RotMat.A[0][0], RotMat.A[0][1], RotMat.A[0][2])),                        /* At */
    Cam->Up);                                                              /* Up */
}  /* End of 'RK2_RndCameraRotateRight' funciton */

/* Camera rotation by camera-axes-dir function.
* ARGUMENTS:
*   - Angle:
*       DBL Angle;
* RETURNS: None.
*/
VOID RK2_RndCameraRotateDir( rk2CAMERA *Cam, DBL Angle )
{
  rk2MATR4x4 RotMat;

  Cam->Dir = VecSubVec(Cam->At, Cam->Loc);
  Cam->Right = VecCrossVec(VecNormalize(Cam->Dir), Cam->Up);
  RK2_RndCameraNormalize(Cam);

  RotMat = RK2_RndCameraGetMatrix(Cam);
  RotMat = MatrMultMatr(MatrRotateX(MatrDefault(), Angle), RotMat);

  RK2_RndCameraSet(Cam, Cam->Loc,                                               /* Location */
    VecSumVec(Cam->Loc,
    VecSet(RotMat.A[0][0], RotMat.A[0][1], RotMat.A[0][2])),                    /* At */
    Cam->Up);                                                                   /* Up */
}  /* End of 'RK2_RndCameraRotateDir' funciton */


/* END OF 'RENDER.C' FILE */
