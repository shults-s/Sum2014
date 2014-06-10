static VOID CubeUnitResponse( vg4UNIT_CUBE *Unit, vg4ANIM *Ani )
{
  VG4_RndWs = Ani->W;
  VG4_RndHs = Ani->H;
} /* End of 'CubeUnitResponse' function */

static VOID CubeUnitRender( vg4UNIT_CUBE *Unit, vg4ANIM *Ani )
{
  INT i, s = 5;
  VEC p = {1, 0, 0};
  POINT pt;

  VG4_RndMatrView = MatrViewLookAt(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  VG4_RndMatrWorld = MatrRotateY(Ani->Time * 30);

  SelectObject(Ani->hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(Ani->hDC, RGB(255, 255, 255));
  srand(30);
  for (i = 0; i < 1000; i++)
  {
    p.X = 2.0 * rand() / RAND_MAX - 1;
    p.Y = 2.0 * rand() / RAND_MAX - 1;
    p.Z = 2.0 * rand() / RAND_MAX - 1;
    pt = VG4_RndWorldToScreen(p);
    Ellipse(Ani->hDC, pt.x - s, pt.y - s, pt.x + s, pt.y + s);
  }
} /* End of 'CubeUnitRender' function */

