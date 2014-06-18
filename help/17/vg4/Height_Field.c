/* Функция создания карты высот.
 * АРГУМЕНТЫ:
 *   - создаваемый примитив:
 *       vg4PRIM *P;
 *   - растровое изображение с высотами:
 *       CHAR *FileName;
 *   - максимальная высота и масштабирование:
 *       FLT Height, Scale;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_PrimCreateHeightField( vg4PRIM *P, CHAR *FileName, FLT Height, FLT Scale )
{
  INT i, j;
  IMAGE Img;

  memset(P, 0, sizeof(vg4PRIM));
  if (!ImageLoad(&Img, FileName))
    return FALSE;

  if (!VG4_PrimCreate(P, VG4_PRIM_GRID, Img.W, Img.H))
    return FALSE;
  /* задаем координаты точек */
  for (i = 0; i < Img.H; i++)
    for (j = 0; j < Img.W; j++)
    {
      vg4VERTEX *V = P->V + i * Img.W + j;
      BYTE rgb[4], h;
      DWORD *col = (DWORD *)rgb;

      *col = ImageGetP(&Img, j, i);
      h = (rgb[2] * 30 + rgb[1] * 59 + rgb[0] * 11) / 100;
      V->P = VecSet((j - Img.W / 2) / Scale, h * Height / 255, (i - Img.H / 2) / Scale);
    }
  /* задаем нормали точек */
  for (i = 1; i < Img.H - 1; i++)
    for (j = 1; j < Img.W - 1; j++)
    {
      VEC
        p00 = P->V[i * Img.W + j].P,
        p0_1 = P->V[i * Img.W + (j - 1)].P,
        p01 = P->V[i * Img.W + (j + 1)].P,
        p_10 = P->V[(i - 1) * Img.W + j].P,
        p10 = P->V[(i + 1) * Img.W + j].P,
        du0 = VecNormalize(VecSet((p0_1.Y - p00.Y), (p00.X - p0_1.X), 0)),
        du1 = VecNormalize(VecSet((p00.Y - p01.Y), (p01.X - p00.X), 0)),
        dv0 = VecNormalize(VecSet(0, (p00.Z - p_10.Z), (p_10.Y - p00.Y))),
        dv1 = VecNormalize(VecSet(0, (p10.Z - p00.Z), (p00.Y - p10.Y)));

      P->V[i * Img.W + j].N = VecNormalize(VecAddVec(VecAddVec(du0, du1), VecAddVec(dv0, dv1)));
      /// P->V[i * Img.W + j].N = VecNormalize(VecAddVec(du0, du1));
    }
  return TRUE;
} /* End of 'VG4_PrimCreateSphere' function */

