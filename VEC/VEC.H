/* SS3 06.06.14 */

#ifndef __VEC_H_
#define __VEC_H_

#define DEGREE2RADIANL 0.01745329251994329576L
long double MultiplierDegree2Radian = DEGREE2RADIANL;

__inline VEC VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

__inline VEC VecLen( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len != 0 && len != 1)
    return sqrt(len);
  return len;
}

/* Единичная матрица */
MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  }
};

MATR MatrIdenity( VOID )
{
  return UnitMatrix;
}

MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR m = UnitMatrix;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
}

MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR m = UnitMatrix;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  return m;
}

MATR MatrRotateX( DBL AngleInDegree )
{
  MATR m;
  DBL a, sine, cosine;

  __asm {
    fld AngleInDegree
    fmul MultiplierDegree2Radian
    fsincos
    fstp cosine
    fstp sine + 88
  }

  m.A[0][0] = 1;
  m.A[0][1] = 0;
  m.A[0][2] = 0;
  m.A[0][3] = 0;
  m.A[1][0] = 0;
  m.A[1][1] = cosine;
  m.A[1][2] = sine;
  m.A[1][3] = 0;
  m.A[2][0] = 0;
  m.A[2][1] = -sine;
  m.A[2][2] = cosine;
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
}

MATR MatrRotateY( DBL AngleInDegree )
{
  MATR m;
  DBL a, sine, cosinene;

  __asm {
    fld AngleInDegree
    fmul MultiplierDegree2Radian
    fsincos 
    fstp cosine
    fstp sine + 88
  }

  m.A[0][0] = cosinene;
  m.A[0][1] = 0;
  m.A[0][2] = -sine;
  m.A[0][3] = 0;
  m.A[1][0] = 0;
  m.A[1][1] = 1;
  m.A[1][2] = 0;
  m.A[1][3] = 0;
  m.A[2][0] = sine;
  m.A[2][1] = 0;
  m.A[2][2] = cosinene;
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
}

MATR MatrRotateZ(DBL AngleInDegree )
{
  MATR m;
  DBL a, sine, cosinene;

  __asm {
    fld AngleInDegree
    fmul MultiplierDegree2Radian
    fsincos 
    fstp cosine
    fstp sine + 88
  }

  m.A[0][0] = cosinene;
  m.A[0][1] = 0;
  m.A[0][2] = -sine;
  m.A[0][3] = 0;
  m.A[1][0] = 0;
  m.A[1][1] = 1;
  m.A[1][2] = 0;
  m.A[1][3] = 0;
  m.A[2][0] = sine;
  m.A[2][1] = 0;
  m.A[2][2] = cosinene;
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
}

MATR MatrRotate( DBL AngleInDegree,
                 DBL X, DBL Y, DBL Z )
{
  DBL a, sine, cosine, len;
  MATR m;

  __asm {
    fld AngleInDegree
    fmul MultiplierDegree2Radian
    fsincos
    fstp cosine
    fstp sine + 88
  }

  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
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

MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR m;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (m.A[i][j] = 0, k = 0; k < 4; k++)
        m.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return m;
}

MATR MatrTranspose( MATR M )
{
  int i;
  MATR m;

  m.A[0][0] = M.A[0][0];
  m.A[0][1] = M.A[1][0];
  m.A[0][2] = M.A[2][0];
  m.A[0][3] = M.A[3][0];

  m.A[1][0] = M.A[0][1];
  m.A[1][1] = M.A[1][1];
  m.A[1][2] = M.A[2][1];
  m.A[1][3] = M.A[3][1];

  m.A[2][0] = M.A[0][2];
  m.A[2][1] = M.A[1][2];
  m.A[2][2] = M.A[2][2];
  m.A[2][3] = M.A[3][2];

  m.A[3][0] = M.A[0][3];
  m.A[3][1] = M.A[1][3];
  m.A[3][2] = M.A[2][3];
  m.A[3][3] = M.A[3][3];

  return m;
}

/* Определитель матрицы 3х3 */
DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}
DBL MatrDeterm( MATR M )
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

MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  INT perm[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  if (det == 0)
    return UnitMatrix;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[j][i] =
        MatrDeterm3x3(
          M.A[perm[i][0]][perm[j][0]],
          M.A[perm[i][0]][perm[j][1]],
          M.A[perm[i][0]][perm[j][2]],
          M.A[perm[i][1]][perm[j][0]],
          M.A[perm[i][1]][perm[j][1]],
          M.A[perm[i][1]][perm[j][2]],
          M.A[perm[i][2]][perm[j][0]],
          M.A[perm[i][2]][perm[j][1]],
          M.A[perm[i][2]][perm[j][2]]) / det;
  return r;
}

#endif