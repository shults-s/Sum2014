/* FILENAME: RENDER.H
 * PROGRAMMER: RK2
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 12.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "image.h"
#include "mth.h"

/* Camera properties */
typedef struct
{
  rk2VEC
    Loc,             /* Location */
    Dir, Up, Right,  /* Orient vectors */
    At;              /* Point to view */
} rk2CAMERA;

/* Global camera */
extern rk2CAMERA RK2_RndCamera;

/* Render setting function. */
VOID RK2_RndSetRenderProp( INT W, INT H );


/***
 * Camera functions
 ***/

/* Setting cameras properties */
VOID RK2_RndCameraSet( rk2CAMERA *Cam, rk2VEC Loc, rk2VEC At, rk2VEC Up );

/* Setting cameras properties */
VOID RK2_RndCameraMoveDir( rk2CAMERA *Cam, DBL Coef );

/* Camera move by Right-Axes function. */
VOID RK2_RndCameraMoveRight( rk2CAMERA *Cam, DBL Coef );

/* Camera move by Up-Axes function. */
VOID RK2_RndCameraMoveUp( rk2CAMERA *Cam, DBL Coef );

/* Camera rotation by axes Y function. */
VOID RK2_RndCameraRotateUp( rk2CAMERA *Cam, DBL Angle );

/* Camera rotation by camera-axes-right function. */
VOID RK2_RndCameraRotateRight( rk2CAMERA *Cam, DBL Angle );

/* Camera rotation by camera-axes-dir function. */
VOID RK2_RndCameraRotateDir( rk2CAMERA *Cam, DBL Angle );

/* Cameras normalizing vectors function */
VOID RK2_RndCameraNormalize( rk2CAMERA *Cam );

/* Camera update information function. */
VOID RK2_RndCameraUpdateInfo( rk2CAMERA *Cam );

/* Render setting function. */
VOID RK2_RndBuildMatrix( VOID );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
