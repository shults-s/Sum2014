/* Функция создания цилиндра.
 * АРГУМЕНТЫ:
 *   - создаваемый примитив:
 *       vg4PRIM *P;
 *   - высота цилиндра:
 *       FLT H;
 *   - радиус цилиндра:
 *       FLT R;
 *   - разбиение сетки (ширина, высота):
 *       INT M, N;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_PrimCreateCylinder( vg4PRIM *P, FLT H, FLT R, INT M, INT N )
{
  INT i, j;

  if (!VG4_PrimCreate(P, VG4_PRIM_GRID, M, N))
    return FALSE;
  /* задаем координаты точек */
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      vg4VERTEX *V = P->V + i * M + j;
      DBL
        phi = j / (M - 1.0) * 2 * VG4_PI;

      V->N = VecSet(0, sin(phi), cos(phi));
      V->P = VecSet(-H / 2 + i * H / (N - 1), V->N.Y * R, V->N.Z * R);
      V->T.U *= 10;
      V->T.V *= 8;  /* количество полос */
    }
  return TRUE;
} /* End of 'VG4_PrimCreateCylinder' function */


VG4_PrimCreateCylinder(&prim, 12, 1, 88, 5);
  mtl.Ka = VecSet(0.1, 0.1, 0.1);
  mtl.Kd = VecSet(1, 1, 1);
  mtl.Ks = VecSet(0, 0, 0);
  mtl.Phong = 30;
  mtl.Trans = 1;
  mtl.TexNo = 0;
  mtl.MapD[0] = 0;
  strcpy(mtl.MapD, "r.bmp");
  strcpy(mtl.Name, "Height Field Material");
  prim.Mtl = VG4_GeomAddMaterial(&Unit->Obj, &mtl);
