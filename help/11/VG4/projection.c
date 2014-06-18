
render:
/* Матрицы */
MATR
  VG4_RndMatrWorld = VG4_UNIT_MATR,      /* матрица преобразования мировой СК */
  VG4_RndMatrView = VG4_UNIT_MATR,       /* матрица преобразования видовой СК */
  VG4_RndMatrProjection = VG4_UNIT_MATR; /* матрица проекции */

static MATR
  VG4_RndMatrWorldViewProj;              /* Итоговая матрица преобразования */

/* Функция перевычисления итоговой матрицы преобразования.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_RndMatrSetup( VOID )
{
  VG4_RndMatrWorldViewProj =
    MatrMulMatr(MatrMulMatr(VG4_RndMatrWorld, VG4_RndMatrView),
      VG4_RndMatrProjection);
} /* End of 'VG4_RndMatrSetup' function */

POINT VG4_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  Pp = VecMulMatr(P, VG4_RndMatrWorldViewProj);
  
  /* проецирование */
  Ps.x = ( Pp.X + 0.5) * (VG4_RndWs - 1);
  Ps.y = (-Pp.Y + 0.5) * (VG4_RndHs - 1);
  return Ps;
} /* End of 'VG4_RndWorldToScreen' function */

VG4_RndGObjDraw:
  . . .
  /* обновляем матрицу преобразования */
  VG4_RndMatrSetup();

  for (i = 0; i < GObj->NumOfV; i++)
  {
    pts[i] = VG4_RndWorldToScreen(GObj->V[i]);
    /* Ellipse(hDC, pts[i].x - s, pts[i].y - s, pts[i].x + s, pts[i].y + s); */
  }
  . . .



__inline MATR MatrProjection( DBL Left, DBL Right,
                              DBL Bottom, DBL Top,
                              DBL Near, DBL Far )
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
  m.A[0][2] = -Dir.X;
  m.A[1][2] = -Dir.Y;
  m.A[2][2] = -Dir.Z;
  m.A[3][2] = - -VecDotVec(Dir, Loc);
  m.A[0][3] = 0;
  m.A[1][3] = 0;
  m.A[2][3] = 0;
  m.A[3][3] = 1;
  return m;
} /* End of 'MatrViewLookAt' function */
