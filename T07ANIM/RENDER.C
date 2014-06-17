/* FILENAME: RENDER.C
 * PROGRAMMER: SS3
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anim.h"
#include "render.h"

BOOL SS3_RndGObjLoad( ss3GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* обнулили данные */
  memset(GObj, 0, sizeof(ss3GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* считаем количество вершин и граней */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* выделяем память под вершины и грани как единый участок памяти (memory bulk) */
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
      if (sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i %i %i", &a, &b, &c))
      {
        GObj->F[nf][0] = a - 1;
        GObj->F[nf][1] = b - 1;
        GObj->F[nf][2] = c - 1;
        nf++;
      }
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
  INT i = 0;
  glBegin(GL_LINES);
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n0 = GObj->F[i][0],
      n1 = GObj->F[i][1],
      n2 = GObj->F[i][2];
    
    glColor3d(1, 1, 1);
    glVertex3d(GObj->V[n0].X, GObj->V[n0].Y, GObj->V[n0].Z);
    glVertex3d(GObj->V[n1].X, GObj->V[n1].Y, GObj->V[n1].Z);
    glColor3d(1, 1, 1);
    glVertex3d(GObj->V[n1].X, GObj->V[n1].Y, GObj->V[n1].Z);
    glVertex3d(GObj->V[n2].X, GObj->V[n2].Y, GObj->V[n2].Z);
    glColor3d(1, 1, 1);
    glVertex3d(GObj->V[n0].X, GObj->V[n0].Y, GObj->V[n0].Z);
    glVertex3d(GObj->V[n2].X, GObj->V[n2].Y, GObj->V[n2].Z);   
  }
  glEnd();
}