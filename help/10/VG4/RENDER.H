/* FILENAME: RENDER.H
 * PROGRAMMER: VG4
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} CAMERA;

/* Глобальная камера */
extern CAMERA VG4_RndCam;

/* Параметры проецирования */
extern DBL
  VG4_RndWs, VG4_RndHs, /* размеры кадра в точках */
  VG4_RndWp, VG4_RndHp, /* размеры обрасти проецирования */
  VG4_RndProjDist;      /* расстояние до плоскости проекции */

/* Матрицы */
extern MATR
  VG4_RndMatrWorld, /* матрица преобразования мировой СК */
  VG4_RndMatrView;  /* матрица преобразования видовой СК */

/* Функция преобразования из мировой системы коорлинат в кадр */
POINT VG4_RndWorldToScreen( VEC P );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
