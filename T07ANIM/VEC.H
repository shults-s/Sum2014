/* FILENAME: VEC.H
 * PROGRAMMER: SS3
 * PURPOSE: 3D math declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>
#include <windows.h>

#include "def.h"


typedef float FLT;
typedef double DBL;

/* Константы и преобразования для углов */
#define SS3_PI 3.14159265358979323846
#define SS3_D2R(A) ((A) * (SS3_PI / 180.0))
#define SS3_R2D(A) ((A) * (180.0 / SS3_PI))

/* Множитель для преобразования градусов в радианы */
#define SS3_DEGREE2RADIANL 0.01745329251994329576L
extern long double SS3_MultiplierDegree2Radian;

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

typedef struct tagCOLOR
{
  FLT R, G, B, A;
} COLOR;

__inline COLOR ColorSet( FLT R, FLT G, FLT B )
{
  COLOR c;

  c.R = R;
  c.B = B;
  c.G = G;
  c.A = 1;
  return c;
}

__inline COLOR ColorSetA( FLT R, FLT G, FLT B, FLT A )
{
  COLOR c;

  c.R = R;
  c.B = B;
  c.G = G;
  c.A = A;
  return c;
}

__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
}

__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}

__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

__inline VEC VecMulNum( VEC V1, FLT N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
}

__inline VEC VecDivNum( VEC V1, FLT N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
}

__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X);
}

__inline FLT VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}
__inline FLT VecLen( VEC V )
{
  FLT len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;

  if (len != 0 && len != 1)
    return sqrt(len);
  return len;
}

__inline VEC VecNormalize( VEC V )
{
  FLT len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;
  if (len != 0 && len != 1)
  {
    len = sqrt(len);
    return VecSet(V.X / len, V.Y / len,  V.Z / len);
  }
  return V;
}

/* умножение вектора на матрицу (с учетом однородной координаты) */
__inline VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}

/* единичная матрица */
#define SS3_UNIT_MATR \
{                 \
  {               \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}  \
  }               \
}

__inline MATR MatrIdentity( VOID )
{
  static MATR m = SS3_UNIT_MATR;

  return m;
}

__inline MATR MatrTranslate( FLT Dx, FLT Dy, FLT Dz )
{
  MATR m = SS3_UNIT_MATR;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
}

__inline MATR MatrScale( FLT Sx, FLT Sy, FLT Sz )
{
  MATR m = SS3_UNIT_MATR;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  return m;
}

__inline MATR MatrRotateX( FLT AngleInDegree )
{
  FLT sine, cosine;
  MATR m = SS3_UNIT_MATR;

  __asm {
    fld  AngleInDegree
    fmul SS3_MultiplierDegree2Radian

    fsincos

    fstp cosine
    fstp sine
  }
  m.A[1][1] = cosine;
  m.A[2][2] = cosine;
  m.A[1][2] = sine;
  m.A[2][1] = -sine;
  return m;
}

__inline MATR MatrRotateY( FLT AngleInDegree )
{
  FLT sine, cosine;
  MATR m = SS3_UNIT_MATR;

  __asm {
    fld  AngleInDegree
    fmul SS3_MultiplierDegree2Radian

    fsincos

    fstp cosine
    fstp sine
  }
  m.A[0][0] = cosine;
  m.A[2][2] = cosine;
  m.A[2][0] = sine;
  m.A[0][2] = -sine;
  return m;
}

__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  FLT sine, cosine;
  MATR m = SS3_UNIT_MATR;

  __asm {
    fld  AngleInDegree
    fmul SS3_MultiplierDegree2Radian

    fsincos

    fstp cosine
    fstp sine
  }
  m.A[0][0] = cosine;
  m.A[1][1] = cosine;
  m.A[0][1] = sine;
  m.A[1][0] = -sine;
  return m;
}

__inline MATR MatrRotate( FLT AngleInDegree,
                          FLT X, FLT Y, FLT Z )
{
  FLT sine, cosine, len;
  MATR m = SS3_UNIT_MATR;

  AngleInDegree /= 2;
  __asm {
    fld  AngleInDegree
    fmul SS3_MultiplierDegree2Radian

    fsincos

    fstp cosine
    fstp sine
  }

  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
  {
    len = sqrt(len);
    X /= len;
    Y /= len;
    Z /= len;
  }

  X *= sine;
  Y *= sine;
  Z *= sine;
  m.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  m.A[0][1] = 2 * X * Y - 2 * cosine * Z;
  m.A[0][2] = 2 * cosine * Y + 2 * X * Z;
  m.A[0][3] = 0;
  m.A[1][0] = 2 * X * Y + 2 * cosine * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * cosine * X;
  m.A[1][3] = 0;
  m.A[2][0] = 2 * X * Z - 2 * cosine * Y;
  m.A[2][1] = 2 * cosine * X + 2 * Y * Z;
  m.A[2][2] = 1 - 2 * (X * X + Y * Y);
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
}
 
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.A[i][j] = 0, k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}
 
__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
}

__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                            FLT A21, FLT A22, FLT A23,
                            FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 -
         A11 * A23 * A32 -
         A12 * A21 * A33 +
         A12 * A23 * A31 +
         A13 * A21 * A32 - 
         A13 * A22 * A31;
}

__inline FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) - 
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  FLT det = MatrDeterm(M);

  if (det == 0)
  {
    static MATR m = SS3_UNIT_MATR;

    return m;
  }

  /* строим присоединенную матрицу (adjoint matrix) */
  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                 M.A[1][1], M.A[1][2], M.A[1][3],
                 M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3]);
 
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]);
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]);
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]);

  /* делим на определитель */
  r.A[0][0] /= det;
  r.A[1][0] /= det;
  r.A[2][0] /= det;
  r.A[3][0] /= det;
  r.A[0][1] /= det;
  r.A[1][1] /= det;
  r.A[2][1] /= det;
  r.A[3][1] /= det;
  r.A[0][2] /= det;
  r.A[1][2] /= det;
  r.A[2][2] /= det;
  r.A[3][2] /= det;
  r.A[0][3] /= det;
  r.A[1][3] /= det;
  r.A[2][3] /= det;
  r.A[3][3] /= det;

  return r;
}

__inline MATR MatrViewLookAt( VEC Loc, VEC At, VEC Up )
{
  VEC
    Dir = VecSubVec(At, Loc),
    Right = VecCrossVec(Dir, Up);
  MATR m;

  Dir = VecNormalize(Dir);
  Right = VecNormalize(Right);
  Up = VecCrossVec(Right, Dir);

  m.A[0][0] = Right.X;
  m.A[1][0] = Right.Y;
  m.A[2][0] = Right.Z;
  m.A[3][0] = -VecDotVec(Right, Loc);
  m.A[0][1] = Up.X;
  m.A[1][1] = Up.Y;
  m.A[2][1] = Up.Z;
  m.A[3][1] = -VecDotVec(Up, Loc);
  m.A[0][2] = -Dir.X;   /**/
  m.A[1][2] = -Dir.Y;   /**/
  m.A[2][2] = -Dir.Z;   /**/
  m.A[3][2] = VecDotVec(Dir, Loc); /**/
  m.A[0][3] = 0;
  m.A[1][3] = 0;
  m.A[2][3] = 0;
  m.A[3][3] = 1;
  return m;
}

__inline MATR MatrProjection( FLT Left, FLT Right,
                              FLT Bottom, FLT Top,
                              FLT Near, FLT Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };
  return m;
}

__inline MATR MatrMult4x4( MATR A, MATR B )
{
  MATR C;
  INT i = 0, x = 0, y = 0;

  for (y = 0; y < 4; y++)
    for (x = 0; x < 4; x++)
    {
      C.A[y][x] = 0;
      for (i = 0; i < 4; i++)
        C.A[y][x] += A.A[y][i] * B.A[i][x]; 
    }
  
  return C;
}

__inline VEC MatrMultVec( MATR A, VEC B )
{
  INT i = 0;
  VEC C;

  C.X = (B.X * A.A[0][0] + B.Y * A.A[1][0] + B.Z * A.A[2][0] + A.A[3][0]);
  C.Y = (B.X * A.A[0][1] + B.Y * A.A[1][1] + B.Z * A.A[2][1] + A.A[3][1]);
  C.Z = (B.X * A.A[0][2] + B.Y * A.A[1][2] + B.Z * A.A[2][2] + A.A[3][2]);

  return C;
}

#endif