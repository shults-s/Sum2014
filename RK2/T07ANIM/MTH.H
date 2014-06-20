/* FILENAME: MTH.H
 * PROGRAMMER: RK2
 * PURPOSE: 3D math declaration module.
 * LAST UPDATE: 17.06.2014
 */

#ifndef __MTH_H_
#define __MTH_H_

#include <math.h>
#include <windows.h>

#pragma warning(disable: 4244 4305)

/* Common float point types */
typedef float FLT;
typedef double DBL;

/* Coords to convert angles */
#define RK2_PI 3.14159265358979323846
#define RK2_D2R(A) ((A) * (RK2_PI / 180.0))
#define RK2_R2D(A) ((A) * (180.0 / RK2_PI))

/* Component to convert degrees to radians */
#define RK2_DEGREE2RADIANL 0.01745329251994329576L
extern long double RK2_MultiplierDegree2Radian;

/* Vector type*/
typedef struct tagrk2rk2VEC
{
  FLT X, Y, Z; /* Vector components */
} rk2VEC;

/* Color struct */
typedef struct tagrk2COLOR
{
  FLT R, G, B, A; /* Color components */
} rk2COLOR;

/* Matrix 4x4 */
typedef struct tagrk2MATR4x4
{
  FLT A[4][4]; /* Matrix elements */
} rk2MATR4x4;


/***
 * Color working functions
 ***/

__inline rk2COLOR ColorSet( FLT R, FLT G, FLT B )
{
  rk2COLOR res;

  res.R = R;
  res.G = G;
  res.B = B;
  return res;
} /* End of 'ColorSet' function */

/***
 * Vector working functions
 ***/

__inline rk2VEC VecSet( FLT X, FLT Y, FLT Z )
{
  rk2VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */

__inline rk2VEC VecSumVec( rk2VEC V1, rk2VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

__inline rk2VEC VecSubVec( rk2VEC V1, rk2VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'VecSubVec' function */

__inline rk2VEC VecMultNum( rk2VEC V1, FLT N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
} /* End of 'VecMulNum' function */

__inline rk2VEC VecDivNum( rk2VEC V1, FLT N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
} /* End of 'VecDivNum' function */

__inline rk2VEC VecNeg( rk2VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

/* скалярное произведение векторов */
__inline FLT VecDotVec( rk2VEC V1, rk2VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecDotVec' function */

/* Vector Cross Vector */
__inline rk2VEC VecCrossVec( rk2VEC V1, rk2VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

/* Len Vector * Len vector */
__inline FLT VecLen2( rk2VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
} /* End of 'VecLen2' function */

/* Vector Len */
__inline FLT VecLen( rk2VEC V )
{
  FLT len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;

  if (len != 0 && len != 1)
    return sqrt(len);
  return len;
} /* End of 'VecLen' function */

/* Vector normalize */
__inline rk2VEC VecNormalize( rk2VEC V )
{
  FLT len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;
  if (len != 0 && len != 1)
  {
    len = sqrt(len);
    return VecSet(V.X / len, V.Y / len,  V.Z / len);
  }
  return V;
} /* End of 'VecNormalize' function */

/* Vec Mult Matrix */
__inline rk2VEC VecMultMatr( rk2VEC V, rk2MATR4x4 M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */

/***
 * Matrix functions
 ***/

/* Default matrix */
#define RK2_UNIT_MATR_DEFAULT \
{                 \
  {               \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}  \
  }               \
}

__inline rk2MATR4x4 MatrDefault( VOID )
{
  static rk2MATR4x4 m = RK2_UNIT_MATR_DEFAULT;

  return m;
} /* End of 'MatrIdentity' function */

__inline rk2MATR4x4 MatrTranslate( rk2MATR4x4 Matr, FLT Dx, FLT Dy, FLT Dz )
{
  /// rk2MATR4x4 m = RK2_UNIT_MATR_DEFAULT;

  Matr.A[3][0] = Dx;
  Matr.A[3][1] = Dy;
  Matr.A[3][2] = Dz;
  return Matr;
} /* End of 'MatrTranslate' function */

__inline rk2MATR4x4 MatrScale( FLT Sx, FLT Sy, FLT Sz )
{
  rk2MATR4x4 m = RK2_UNIT_MATR_DEFAULT;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  return m;
} /* End of 'MatrScale' function */

__inline rk2MATR4x4 MatrRotateX( rk2MATR4x4 SrcMatr, FLT AngleInDegree )
{
  FLT sine, cosine;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul RK2_MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  SrcMatr.A[1][1] = cosine;
  SrcMatr.A[2][2] = cosine;
  SrcMatr.A[1][2] = sine;
  SrcMatr.A[2][1] = -sine;
  return SrcMatr;
} /* End of 'MatrRotateX' function */

__inline rk2MATR4x4 MatrRotateY( rk2MATR4x4 SrcMatr, FLT AngleInDegree )
{
  FLT sine, cosine;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul RK2_MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  SrcMatr.A[0][0] = cosine;
  SrcMatr.A[2][2] = cosine;
  SrcMatr.A[2][0] = sine;
  SrcMatr.A[0][2] = -sine;
  return SrcMatr;
} /* End of 'MatrRotateY' function */

__inline rk2MATR4x4 MatrRotateZ( rk2MATR4x4 SrcMatr, FLT AngleInDegree )
{
  FLT sine, cosine;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul RK2_MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  SrcMatr.A[0][0] = cosine;
  SrcMatr.A[1][1] = cosine;
  SrcMatr.A[0][1] = sine;
  SrcMatr.A[1][0] = -sine;
  return SrcMatr;
} /* End of 'MatrRotateZ' function */

__inline rk2MATR4x4 MatrRotate( rk2MATR4x4 SrcMatr, FLT AngleInDegree,
                                FLT X, FLT Y, FLT Z )
{
  FLT sine, cosine, len;
  
  AngleInDegree /= 2;
  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul RK2_MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }

  /* определили длину вектора */
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
  SrcMatr.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  SrcMatr.A[0][1] = 2 * X * Y - 2 * cosine * Z;
  SrcMatr.A[0][2] = 2 * cosine * Y + 2 * X * Z;
  SrcMatr.A[0][3] = 0;
  SrcMatr.A[1][0] = 2 * X * Y + 2 * cosine * Z;
  SrcMatr.A[1][1] = 1 - 2 * (X * X + Z * Z);
  SrcMatr.A[1][2] = 2 * Y * Z - 2 * cosine * X;
  SrcMatr.A[1][3] = 0;
  SrcMatr.A[2][0] = 2 * X * Z - 2 * cosine * Y;
  SrcMatr.A[2][1] = 2 * cosine * X + 2 * Y * Z;
  SrcMatr.A[2][2] = 1 - 2 * (X * X + Y * Y);
  SrcMatr.A[2][3] = 0;
  SrcMatr.A[3][0] = 0;
  SrcMatr.A[3][1] = 0;
  SrcMatr.A[3][2] = 0;
  SrcMatr.A[3][3] = 1;
  return SrcMatr;
} /* End of 'MatrRotate' function */
 
__inline rk2MATR4x4 MatrMultMatr( rk2MATR4x4 M1, rk2MATR4x4 M2 )
{
  INT i, j, k;
  rk2MATR4x4 r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.A[i][j] = 0, k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* End of 'MatrMultMatr' function */
 
__inline rk2MATR4x4 MatrTranspose( rk2MATR4x4 M )
{
  INT i, j;
  rk2MATR4x4 r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
} /* End of 'MatrTranspose' function */

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
} /* End of 'MatrDeterm3x3' function */

__inline FLT MatrDeterm( rk2MATR4x4 M )
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
} /* End of 'MatrDeterm' function */

__inline rk2MATR4x4 MatrInverse( rk2MATR4x4 M )
{
  rk2MATR4x4 r;
  FLT det = MatrDeterm(M);

  if (det == 0)
  {
    static rk2MATR4x4 m = RK2_UNIT_MATR_DEFAULT;

    return m;
  }

  /* Biulding matrix */
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

  /* del to determ */
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
} /* End of 'MatrInverse' function */

/* Setting matrix view function */
__inline rk2MATR4x4 MatrViewLookAt( rk2VEC Loc, rk2VEC At, rk2VEC Up )
{
  rk2VEC
    Dir = VecSubVec(At, Loc),
    Right = VecCrossVec(Dir, Up);
  rk2MATR4x4 m;

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
  m.A[0][2] = -Dir.X;
  m.A[1][2] = -Dir.Y;
  m.A[2][2] = -Dir.Z;
  m.A[3][2] = VecDotVec(Dir, Loc);
  m.A[0][3] = 0;
  m.A[1][3] = 0;
  m.A[2][3] = 0;
  m.A[3][3] = 1;
  return m;
} /* End of 'MatrViewLookAt' function */

__inline rk2MATR4x4 MatrProjection( FLT Left, FLT Right,
                              FLT Bottom, FLT Top,
                              FLT Near, FLT Far )
{
  rk2MATR4x4 m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };
  return m;
} /* End of 'MatrProjection' function */

#endif /* __MTH_H_ */

/* END OF 'MTH.H' FILE */
