/* SS3 06.06.14 */

#include <math.h>
#define SQR(X) ((X) * (X))
#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))

typedef double DBL;

typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR A =
  {
    {
      { 1,  0,  0, 0},
      { 0,  1,  0, 0},
      { 0,  0,  1, 0},
      { Dx, Dy, Dz, 1},
    }
  };
  
  return A;
}

VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
}

VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X,
                V1.Y + V2.Y,
                V1.Z + V2.Z);
}

VEC VecSubVec( VEC V1, VEC V2 )
{
  VEC V;

  V.X = V1.X - V2.X;
  V.Y = V1.Y - V2.Y;
  V.Z = V1.Z - V2.Z;

  return V;
}

VEC VecMulNum( VEC V, DBL N )
{
  V.X *= N;
  V.Y *= N;
  V.Z *= N;

  return V;     
}

VEC VecDivNum( VEC V, DBL N )
{
  if (N != 0)
  {
    V.X /= N;
    V.Y /= N;
    V.Z /= N;
  }

  return V;     
}

VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
}

DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.X * V2.Z - V1.Z * V2.X,
                V1.X * V2.Y - V1.Y * V2.X);
}

DBL VecLen2( VEC V )
{
  return SQR(V.X) * SQR(V.Y) * SQR(V.Z);
}

DBL VecLen( VEC V )
{
  return sqrt(SQR(V.X) * SQR(V.Y));
}

VEC VecNormalize( VEC V )
{
  DBL len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;

  if (len != 0 && len != 1)
  {
    len = sqrt(len);
    V.X /= len;
    V.Y /= len;
    V.Z /= len;
  }
    
  return V;
}

VEC PointTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}

VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}