/* FILENAME: RENDER.C
 * PROGRAMMER: SS3
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"

/* Глобальная камера */
ss3CAMERA SS3_RndCam;

DBL
  SS3_RndWs = 30, SS3_RndHs = 30,
                 // Размер коровы
  SS3_RndWp = 4, SS3_RndHp = 0.3,
  SS3_RndProjDist = 5;

/* Матрицы */
MATR
  SS3_RndMatrWorld = SS3_UNIT_MATR,
  SS3_RndMatrView = SS3_UNIT_MATR;

POINT SS3_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  Pp = VecMulMatr(P, MatrMulMatr(SS3_RndMatrWorld, SS3_RndMatrView));
  
  /* проецирование */
  Pp.X *= SS3_RndProjDist / Pp.Z;
  Pp.Y *= SS3_RndProjDist / Pp.Z;

  Ps.x = ( Pp.X + SS3_RndWp / 2) / SS3_RndWp * (SS3_RndWs - 1);
  Ps.y = (-Pp.Y + SS3_RndHp / 2) / SS3_RndHp * (SS3_RndHs - 1);
  return Ps;
}

BOOL SS3_RndGObjLoad( ss3GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  memset(GObj, 0, sizeof(ss3GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* считываем данные */
  nv = 0;
  nf = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    DBL x, y, z;
    INT a, b, c;

    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      GObj->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
      sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf + 2, "%i%i%i", &a, &b, &c) == 3;
      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  return TRUE;
}

VOID SS3_RndGObjFree( ss3GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(ss3GOBJ));
}

VOID SS3_RndGObjDraw( ss3GOBJ *GObj, HDC hDC )
{
  INT i, j, s = 1;
  POINT pt[3];

  for (i = 0; i < GObj->NumOfV; i++)
  {
    pt[0] = SS3_RndWorldToScreen(GObj->V[i]);
    Ellipse(hDC, pt[0].x - s, pt[0].y - s, pt[0].x + s, pt[0].y + s);
  }
  for (i = 0; i < GObj->NumOfF; i++)
  {
    for (j = 0; j < 3; j++)
      pt[j] = SS3_RndWorldToScreen(GObj->V[GObj->F[i][j]]);
    MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
    LineTo(hDC, pt[1].x, pt[1].y);
    LineTo(hDC, pt[2].x, pt[2].y);
    LineTo(hDC, pt[0].x, pt[0].y);
  }
}