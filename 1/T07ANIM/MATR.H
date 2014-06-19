/* FILENAME: MATR.C
 * PROGRAMMER: EF2
 * PURPOSE: Matrix defenition module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __MATR_H_
#define __MATR_H_

#include <math.h>
#include <windows.h>

#define EF2_PI 3.14159265358979323846
#define EF2_D2R(A) ((A) * (EF2_PI / 180.0))
#define EF2_R2D(A) ((A) * (180.0 / EF2_PI))
#pragma warning(disable: 4244 4305)

/* Common float point types */
typedef float FLT;
typedef double DBL;



/*****
 ** Color module function
 *****/

/* Main color data type */
typedef struct tagCOLOR
{
  FLT R, G, B, A;  /* components of color */
} COLOR;

__inline COLOR ColorSet( FLT R, FLT G, FLT B )
{
  COLOR c;

  c.R = R;
  c.B = B;
  c.G = G;
  c.A = 1;
  return c;
} /* End of 'ColorSet' function */

__inline COLOR ColorSetA( FLT R, FLT G, FLT B, FLT A )
{
  COLOR c;

  c.R = R;
  c.B = B;
  c.G = G;
  c.A = A;
  return c;
} /* End of 'ColorSetA' function */

/* Main vector data type */
typedef struct tagVEC
{
  FLT X, Y, Z; /* Vector information */
} VEC;

/* Main matrix data type */
typedef struct tagMATR
{
  FLT A[4][4];  /* Matrix information */
} MATR;


/* Mult two matrix function */
MATR EF2_MatrMult4x4( MATR A, MATR B );

/* Create vector function.
 * ARGUMENTS:
 *   - coordinates of vector:
 *       FLT X, Y, Z;
 * RETURNS:
 *   (VEC) Vector in his data type.
 */
__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */

/* Sum two vectors function.
 * ARGUMENTS:
 *   - two vectors for action:
 *       VEC V1, VEC V2;
 * RETURNS:
 *   (VEC) Result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X,
                V1.Y + V2.Y,
                V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

/* Sub two vectors function.
 * ARGUMENTS:
 *   - two vectors for action:
 *       VEC V1, VEC V2;
 * RETURNS:
 *   (VEC) Result vector.
 */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X,
                V1.Y - V2.Y,
                V1.Z - V2.Z);
} /* End of 'VecSubVec' function */

/* Mul vector for a number function.
 * ARGUMENTS:
 *   - vector for action:
 *       VEC V;
 *   - number for action:
 *       BDL N;
 * RETURNS:
 *   (VEC) Result vector.
 */
__inline VEC VecMulNum( VEC V, FLT N )
{
  return VecSet(V.X * N, V.Y * N, V.Z * N);
} /* End of 'VecMulNum' function */

/* Div vectir for a number function.
 * ARGUMENTS:
 *   - vector for action:
 *       VEC V;
 *   - number for action:
 *       BDL N;
 * RETURNS:
 *   (VEC) Result vector.
 */
__inline VEC VecDivNum( VEC V, FLT N )
{
  return VecSet(V.X / N, V.Y / N, V.Z / N);
} /* End of 'VecDivNum' function */

/* Create 'negative' vector function.
 * ARGUMETNS:
 *   - vector for action:
 *       VEC V;
 * RETURNS:
 *   (VEC) Result vector.
 */
__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

/* Dot vector for vector function.
 * ARGUMENTS:
 *   - vectors for action:
 *       VEC V1, V2;
 * RETURNS:
 *   (FLT) Result number.
 */
__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecDotVec' function */

/* Do vector mult for vectors function
 * ARGUMENTS:
 *   - vectors for action:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) Result vector.
 */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

/* Do normalize fo vector function.
 * ARGUMETNS:
 *   - vectors for action:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) Result vector.
 */
__inline VEC VecNormalize( VEC V )
{
  FLT len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;
  if (len != 0 && len != 1)
  {
    len = sqrt(len);
    return VecSet(V.X / len, V.Y / len,  V.Z / len);
  }
  return V;
} /* End of 'VecNormalize' function */

/* Get Unity Matrix function */
MATR MatrIdenity( VOID );

/* Scale matrix function.
 * ARGUMENTS:
 *   - scaled coff:
 *       FLT Sx, Sy, Sz;
 * RETURNS:
 *   (MATR) Result matrix.
 */
__inline MATR MatrScale( FLT Sx, FLT Sy, FLT Sz )
{
  MATR m = MatrIdenity();

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  return m;
} /* End of 'MatrScale' function */

__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
} /* End of 'MatrTranspose' function */

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                            DBL A21, DBL A22, DBL A23,
                            DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 -
         A11 * A23 * A32 -
         A12 * A21 * A33 +
         A12 * A23 * A31 +
         A13 * A21 * A32 - 
         A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

__inline DBL MatrDeterm( MATR M )
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

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);

  if (det == 0)
  {
    static MATR m =
    {                        
      {                      
        {1, 0, 0, 0},        
        {0, 1, 0, 0},        
        {0, 0, 1, 0},        
        {0, 0, 0, 1},        
      }                      
    };                        


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
} /* End of 'MatrInverse' function */

/* Translate matrix function.
 * ARGUMENTS:
 *   - numvers for translate:
 *       FLT Dx, Dy, Dz;
 * RETURNS:
 *   (MATR) Result matrix.
 */
__inline MATR MatrTranslate( FLT Dx, FLT Dy, FLT Dz )
{
  MATR m = MatrIdenity();

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
} /* End of 'MatrTranslate' function */

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
} /* End of 'MatrProjection' function */

/* Mult matrix for vector function */
VEC EF2_MatrMultVec( MATR A, VEC B );

/* Give matrix for position function */
MATR EF2_MatrViewLookAt( VEC Loc, VEC At, VEC Up );

/* Rotate matrix by Y function */
MATR EF2_MatrRotateY( FLT AngleInDegree );

/* Rotate matrix by Z function */
MATR EF2_MatrRotateZ( FLT AngleInDegree );

/* Rotate matrix by X function */
MATR EF2_MatrRotateX( FLT AngleInDegree );


#endif /* __MATR_H_ */

/* END OF 'MATR.H' FILE */
