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
  SS3_RndWp = 4, SS3_RndHp = 1,  /*0.3*/
  SS3_RndProjDist = 5;

/* Матрицы */
MATR
  SS3_RndMatrWorld = SS3_UNIT_MATR,
  SS3_RndMatrView = SS3_UNIT_MATR,
  SS3_RndMatrProjection = SS3_UNIT_MATR;

static MATR
  SS3_RndMatrWorldViewProj;

VOID SS3_RndMatrSetup( VOID )
{
  SS3_RndMatrWorldViewProj =
    MatrMulMatr(MatrMulMatr(SS3_RndMatrWorld, SS3_RndMatrView),
      SS3_RndMatrProjection);
}

POINT SS3_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  Pp = VecMulMatr(P, SS3_RndMatrWorldViewProj);
  
  /* проецирование */
  Ps.x = (Pp.X + SS3_RndWp / 2) / SS3_RndWp * (SS3_RndWs - 1);
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
  INT i;
  POINT *pts;

  if ((pts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;

  /* обновляем матрицу преобразования */
  SS3_RndMatrSetup();

  for (i = 0; i < GObj->NumOfV; i++)
    pts[i] = SS3_RndWorldToScreen(GObj->V[i]);

  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n0 = GObj->F[i][0],
      n1 = GObj->F[i][1],
      n2 = GObj->F[i][2];

    MoveToEx(hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(hDC, pts[n1].x, pts[n1].y);
    LineTo(hDC, pts[n2].x, pts[n2].y);
    LineTo(hDC, pts[n0].x, pts[n0].y);
  }
}