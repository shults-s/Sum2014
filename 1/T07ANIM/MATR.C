/* FILENAME: MATR.C
 * PROGRAMMER: EF2
 * PURPOSE: Matrix defenition module.
 * LAST UPDATE: 06.06.2014
 */

#include "matr.h"
#pragma warning(disable: 4244 4305)

/* Unity matrix */
MATR UnitMatrix =        
{                        
  {                      
    {1, 0, 0, 0},        
    {0, 1, 0, 0},        
    {0, 0, 1, 0},        
    {0, 0, 0, 1},        
  }                      
};                        

/* Mult two matrix function.
 * ARGUMETNS:
 *   - two matrix for mult:
 *       MATR A, MATR B;
 * RETURNS:
 *   (MATR) Matrix after mult
 */
MATR EF2_MatrMult4x4( MATR A, MATR B )
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
} /* End of 'EF2_MatrMult4x4' function*/

/* Mult matrix for vector function.
 * ARGUMENTS:
 *   - matrix for milt:
 *       MATR A;
 *   - vectore for mult:
 *       VEC B;
 * RETURNS:
 *   (VEC) Vectore afret mult
 */
VEC EF2_MatrMultVec( MATR A, VEC B )
{
  INT i = 0;
  VEC C;
 // FLT w = B.X * A.A[0][3] + B.Y * A.A[1][3] + B.Z * A.A[2][3] + A.A[3][3];

  C.X = (B.X * A.A[0][0] + B.Y * A.A[1][0] + B.Z * A.A[2][0] + A.A[3][0]);// / w;
  C.Y = (B.X * A.A[0][1] + B.Y * A.A[1][1] + B.Z * A.A[2][1] + A.A[3][1]);// / w;
  C.Z = (B.X * A.A[0][2] + B.Y * A.A[1][2] + B.Z * A.A[2][2] + A.A[3][2]);// / w;

  return C;
} /* End of 'EF2_MatrMultVec' function */

/* Rotate matrix function.
 * ARGUMENTS:
 *   - angle:
 *       FLT AngleInDegree;
 *   - coordinates of center point:
 *       FLT X, Y, Z;
 * RETURNS:
 *   (MATR) Result matrix;
 */
MATR EF2_MatrRotate( FLT AngleInDegree,
                     FLT X, FLT Y, FLT Z )
{
  FLT a, si, co, len;
  MATR m;

  a = EF2_D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;
  m.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  m.A[0][1] = 2 * X * Y - 2 * co * Z;
  m.A[0][2] = 2 * co * Y + 2 * X * Z;
  m.A[0][3] = 0;
  m.A[1][0] = 2 * X * Y + 2 * co * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * co * X;
  m.A[1][3] = 0;
  m.A[2][0] = 2 * X * Z - 2 * co * Y;
  m.A[2][1] = 2 * co * X + 2 * Y * Z;
  m.A[2][2] = 1 - 2 * (X * X + Y * Y);
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
} /* End of 'EF2_MatrRotate' function */

/* Get Unity Matrix function
 * ARGUMENTS: None.
 * RETURNS:
 *   (MATR) Returned UnitMatrix.
 */
MATR MatrIdenity( VOID )
{
  return UnitMatrix;
} /* End of 'MatrIdenity' function */

/* Give matrix for position function.
 * ARGUMENTS:
 *   - vectors of position:
 *       VEC Loc, At, Up
 * RETURNS:
 *   (MATR) Result matrix.
 */
MATR EF2_MatrViewLookAt( VEC Loc, VEC At, VEC Up )
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
  m.A[3][0] = -VecDotVec(Loc, Right);
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
} /* End of 'EF2_MatrViewLookAt' function */

/* Rotate matrix by Z function.
 * ARGUMENTS:
 *   - angle in degree:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) Result matrix.
 */
MATR EF2_MatrRotateZ( FLT AngleInDegree )
{
  FLT sine, cosine, angle;
  MATR m = MatrIdenity();

  angle = EF2_D2R(AngleInDegree);
  sine = sin(angle);
  cosine = cos(angle);

  m.A[0][0] = cosine;
  m.A[1][1] = cosine;
  m.A[0][1] = sine;
  m.A[1][0] = -sine;
  return m;
} /* End of 'EF2_MatrRotateZ' function */

/* Rotate matrix by Y function.
 * ARGUMENTS:
 *   - angle in degree:
 *       FLT AngleInGegree;
 * RETURNS:
 *   (MATR) Result matrix.
 */
MATR EF2_MatrRotateY( FLT AngleInDegree )
{
  FLT sine, cosine, angle = EF2_D2R(AngleInDegree);
  MATR m = MatrIdenity();

  sine = sin(angle);
  cosine = cos(angle);

  m.A[0][0] = cosine;
  m.A[2][2] = cosine;
  m.A[2][0] = sine;
  m.A[0][2] = -sine;
  return m;
} /* End of 'MatrRotateY' function */

/* Rotate matrix by X function.
 * ARGUMENTS:
 *   - angle in degree:
 *       FLT AngleInGegree;
 * RETURNS:
 *   (MATR) Result matrix.
 */
MATR EF2_MatrRotateX( FLT AngleInDegree )
{
  FLT sine, cosine, angle = EF2_D2R(AngleInDegree);
  MATR m = MatrIdenity();

  sine = sin(angle);
  cosine = cos(angle);

  m.A[1][1] = cosine;
  m.A[2][2] = cosine;
  m.A[1][2] = sine;
  m.A[2][1] = -sine;
  return m;
} /* End of 'MatrRotateX' function */

/* END OF 'MATR.C' FILE */
