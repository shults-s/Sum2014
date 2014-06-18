VEC PointTransform( VEC V, MATR M )
{
  DBL w = V.x * M.A[0][3] + V.y * M.A[1][3] + V.z * M.A[2][3] + M.A[3][3];

  return VecSet((V.x * M.A[0][0] + V.y * M.A[1][0] + V.z * M.A[2][0] + M.A[3][0]) / w,
                (V.x * M.A[0][1] + V.y * M.A[1][1] + V.z * M.A[2][1] + M.A[3][1]) / w,
                (V.x * M.A[0][2] + V.y * M.A[1][2] + V.z * M.A[2][2] + M.A[3][2]) / w);
}

AS4_RndMatrView = MatrViewLookAt(VecTransform(VecSet(0, 0, 15), MatrRotateX(0)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  AS4_RndMatrWorld = MatrRotateY(-cl.x / 3.0);
  AS4_RndMatrWorld = MatrMulMatr(AS4_RndMatrWorld, MatrRotateX(-cl.y / 3.0));
  AS4_RndMatrWorld = MatrMulMatr(AS4_RndMatrWorld, MatrScale(3.5 - AS4_Anim.JPOV / 8.0, 3.5 - AS4_Anim.JPOV / 8.0, 3.5 - AS4_Anim.JPOV / 8.0));