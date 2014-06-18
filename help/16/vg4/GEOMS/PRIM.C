/* FILENAME: PRIM.C
 * PROGRAMMER: VG4
 * PURPOSE: Primtive handle functions.
 * LAST UPDATE: 16.06.2014
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "anim.h"

/* Степень */
#define power(A, B) ((A) < 0 ? -pow(-A, B) : pow(A, B))

/* Цвет по-умолчанию */
COLOR VG4_DefaultColor = {1, 1, 1, 1};

/* Функция создания примитива.
 * АРГУМЕНТЫ:
 *   - создаваемый примитив:
 *       vg4PRIM *P;
 *   - тип примитива:
 *       vg4PRIM_TYPE Type;
 *   - разбиение сетки (ширина, высота) при
 *     Type == VG4_PRIM_GRID или количество вершин
 *     и индексов:
 *       INT NumOfV_GW, NumOfI_GH;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_PrimCreate( vg4PRIM *P, vg4PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH )
{
  INT i, j, nv, ni, size;

  memset(P, 0, sizeof(vg4PRIM));
  /* определение количеств */
  switch (Type)
  {
  case VG4_PRIM_TRIMESH:
    nv = NumOfV_GW;
    ni = NumOfI_GH;
    break;
  case VG4_PRIM_GRID:
    nv = NumOfV_GW * NumOfI_GH;
    ni = NumOfV_GW * 2;
    break;
  }
  /* выделили память */
  size = sizeof(vg4VERTEX) * nv + sizeof(INT) * ni;
  P->V = malloc(size);
  if (P->V == NULL)
    return FALSE;
  memset(P->V, 0, size);
  
  /* заполняем примитив */
  P->Size = size;
  P->Type = Type;
  P->NumOfV = nv;
  P->NumOfI = ni;
  P->GW = NumOfV_GW;
  P->GH = NumOfI_GH;
  P->I = (INT *)(P->V + nv);
  /* Заполняем все вершины по-умолчанию */
  for (i = 0; i < nv; i++)
  {
    P->V[i].C = VG4_DefaultColor;
    P->V[i].N = VecSet(0, 1, 0);
  }

  /* Заполняем индексы и текстуру для рег. сетки */
  if (Type == VG4_PRIM_GRID)
  {
    for (i = 0; i < NumOfI_GH; i++)
      for (j = 0; j < NumOfV_GW; j++)
        P->V[i * NumOfV_GW + j].T =
          VG4_UVSet(j / (NumOfV_GW - 1.0),
                    i / (NumOfI_GH - 1.0));
    for (i = 0; i < NumOfV_GW; i++)
    {
      P->I[2 * i + 0] = NumOfV_GW + i;
      P->I[2 * i + 1] = i;
    }
  }
  return TRUE;
} /* End of 'VG4_PrimCreate' function */

/* Функция создания сферы.
 * АРГУМЕНТЫ:
 *   - создаваемый примитив:
 *       vg4PRIM *P;
 *   - центр сферы:
 *       VEC C;
 *   - радиус сферы:
 *       FLT R;
 *   - разбиение сетки (ширина, высота):
 *       INT M, N;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_PrimCreateSphere( vg4PRIM *P, VEC C, FLT R, INT M, INT N )
{
  INT i, j;
  DBL t = clock() / 1000.0, t1;

  t1 = 2 + 2 * cos(t);
  t = 2 + 2 * sin(t * 1.01 + 2);

  if (!VG4_PrimCreate(P, VG4_PRIM_GRID, M, N))
    return FALSE;
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      vg4VERTEX *V = P->V + i * M + j;
      DBL
        theta = i / (N - 1.0) * VG4_PI,
        phi = j / (M - 1.0) * 2 * VG4_PI;

      V->N = VecSet(power(sin(theta), t1) * power(sin(phi), t),
                    power(cos(theta), t1),
                    power(sin(theta), t1) * power(cos(phi), t));
      V->P = VecAddVec(VecMulNum(V->N, R), C);
    }
  return TRUE;
} /* End of 'VG4_PrimCreateSphere' function */

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - удаляемый примитив:
 *       vg4PRIM *P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_PrimFree( vg4PRIM *P )
{
  if (P->V != NULL)
    free(P->V);
  memset(P, 0, sizeof(vg4PRIM));
} /* End of 'VG4_PrimFree' function */

/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - примитив:
 *       vg4PRIM *P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_PrimDraw( vg4PRIM *P )
{
  INT i;

  if (P->V == NULL)
    return;

  if (P->Buffers[0] == 0)
  {
    /* создаем и заполняем буфера данных */
    glGenBuffers(2, P->Buffers);
    glGenVertexArrays(1, &P->VertexBuffer);

    /* вершины */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vg4VERTEX) * P->NumOfV,
      P->V, GL_STATIC_DRAW);

    /* индексы */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(INT) * P->NumOfI,
      P->I, GL_STATIC_DRAW);
  }

  /* активируем буфера */
  glBindVertexArray(P->VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, P->Buffers[1]);

  /* задаем порядок данных */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(vg4VERTEX), (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(vg4VERTEX), (VOID *)(sizeof(VEC)));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(vg4VERTEX), (VOID *)(sizeof(VEC) + sizeof(vg4UV)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(vg4VERTEX), (VOID *)(2 * sizeof(VEC) + sizeof(vg4UV)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glUseProgram(VG4_ShaderProg);
  if (P->Type == VG4_PRIM_TRIMESH)
  {
    /*
    glBegin(GL_TRIANGLES);
    for (i = 0; i < P->NumOfI; i++)
      glVertex3fv(&P->V[P->I[i]].P.X);
    glEnd();
    */
    glDrawElements(GL_TRIANGLES, P->NumOfI, GL_UNSIGNED_INT, (VOID *)0);
  }
  else
  {
    for (i = 0; i < P->GH - 1; i++)
    {
      /*
      INT start = i * P->GW, j;

      glBegin(GL_TRIANGLE_STRIP);
      for (j = 0; j < P->NumOfI; j++)
        glVertex3fv(&P->V[start + P->I[j]].P.X);
      glEnd();
      */
      glDrawElements(GL_TRIANGLE_STRIP, P->NumOfI, GL_UNSIGNED_INT,
        (VOID *)(i * P->GW * 2 * sizeof(INT)));
    }
  }
  glUseProgram(0);
} /* End of 'VG4_PrimFree' function */

/* END OF 'PRIM.C' FILE */
