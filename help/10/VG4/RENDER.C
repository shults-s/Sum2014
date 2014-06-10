/* FILENAME: RENDER.C
 * PROGRAMMER: VG4
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 10.06.2014
 */

#include "render.h"

/* Глобальная камера */
vg4CAMERA VG4_RndCam;

/* Параметры проецирования */
DBL
  VG4_RndWs = 30, VG4_RndHs = 30,   /* размеры кадра в точках */
  VG4_RndWp = 4, VG4_RndHp = 3,     /* размеры обрасти проецирования */
  VG4_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Матрицы */
MATR
  VG4_RndMatrWorld = VG4_UNIT_MATR, /* матрица преобразования мировой СК */
  VG4_RndMatrView = VG4_UNIT_MATR;  /* матрица преобразования видовой СК */

/* Функция преобразования из мировой системы коорлинат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT VG4_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  Pp = VecMulMatr(P, MatrMulMatr(VG4_RndMatrWorld, VG4_RndMatrView));
  
  /* проецирование */
  Ps.x = ( Pp.X + VG4_RndWp / 2) / VG4_RndWp * (VG4_RndWs - 1);
  Ps.y = (-Pp.Y + VG4_RndHp / 2) / VG4_RndHp * (VG4_RndHs - 1);
  return Ps;
} /* End of 'VG4_RndWorldToScreen' function */

/* END OF 'RENDER.C' FILE */
