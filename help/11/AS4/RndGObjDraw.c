VOID AS4_RndGObjDraw( as4GOBJ *GObj, HDC hDC )
{
  INT i, j, s = 2;
  POINT pt[3];

  for (i = 0; i < GObj->NumOfV; i++)
  {
    pt[0] = AS4_RndWorldToScreen(GObj->V[i]);
    Ellipse(hDC, pt[0].x - s, pt[0].y - s, pt[0].x + s, pt[0].y + s);
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(24, 55, 24));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(255, 153, 24));
  for (i = 0; i < GObj->NumOfF; i++)
  {
    for (j = 0; j < 3; j++)
      pt[j] = AS4_RndWorldToScreen(GObj->V[GObj->F[i][j]]);
    MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
    LineTo(hDC, pt[1].x, pt[1].y);
    LineTo(hDC, pt[2].x, pt[2].y);
    LineTo(hDC, pt[0].x, pt[0].y);/**/
    //Polygon(hDC, pt, 3);
  }
} /* End of 'AS4_RndGObjDraw' function */


AS4_RndMatrView = MatrViewLookAt(VecTransform(VecSet(0, 0, AS4_Anim.JX + 15), MatrRotateX(-cl.y / 3)), VecSet(0, 0, 0), VecSet(0, 1, 0));
AS4_RndMatrWorld = MatrRotateY(-cl.x / 3/*Ani->GlobalTime * 20*/);
AS4_RndMatrWorld = MatrMulMatr(AS4_RndMatrWorld, MatrScale(3.5 - AS4_Anim.JPOV / 10.0, 3.5 - AS4_Anim.JPOV / 10.0, 3.5 - AS4_Anim.JPOV / 10.0));