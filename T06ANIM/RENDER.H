/* FILENAME: RENDER.H
 * PROGRAMMER: SS3
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

typedef struct
{
  VEC
    Loc,
    Dir, Up, Right,
    At;
} ss3CAMERA;

typedef struct tagss3GOBJ
{
  VEC *V;
  INT NumOfV;
  INT (*F)[3];
  INT NumOfF;
} ss3GOBJ;

/* Глобальная камера */
extern ss3CAMERA SS3_RndCam;

/* Параметры проецирования */
extern DBL
  SS3_RndWs, SS3_RndHs,
  SS3_RndWp, SS3_RndHp,
  SS3_RndProjDist;

extern MATR
  SS3_RndMatrWorld,
  SS3_RndMatrView;

/* Функция преобразования из мировой системы коорлинат в кадр */
POINT SS3_RndWorldToScreen( VEC P );

/* Функция загрузки геометрического объекта */
BOOL SS3_RndGObjLoad( ss3GOBJ *GObj, CHAR *FileName );

/* Функция освобождения геометрического объекта */
VOID SS3_RndGObjFree( ss3GOBJ *GObj );

/* Функция отрисовки геометрического объекта */
VOID SS3_RndGObjDraw( ss3GOBJ *GObj, HDC hDC );

#endif