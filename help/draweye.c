/* ������� ��������� ���������� ����� �� 0 �� 1.
 * ���������: ���.
 * ������������ ��������:
 *   (float) - ���������.
 */
float r0( void )
{
  return (float)rand() / RAND_MAX;
} /* End of 'r0' function */

/* ������� ��������� �����.
 * ���������:
 *   - �������� ���������:
 *       HDC hDC;
 *   - ������� ������� ����:
 *       INT Xm, Ym;
 *   - ������� ������ "�����":
 *       INT X�, Y�;
 *   - ������ ����� (������ � ������):
 *       INT W, H;
 *   - ������ "������":
 *       INT R;
 * ������������ ��������: ���.
 */
void DrawEye( HDC hDC, INT Xm, INT Ym, INT Xc, INT Yc, INT W, INT H, INT R )
{
  INT xx, yy;
  float
    len, si, co,
    ratio = (float)W / H;

  len = sqrt(SQR(Xc - Xm) + SQR((Yc - Ym) * ratio));

  if (len == 0)
    si = 0, co = 0;
  else
    co = (Xm - Xc) / len, si = (Ym - Yc) * ratio / len;

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  Ellipse(hDC, Xc - W / 2, Yc - H / 2, Xc + W / 2, Yc + H / 2);

  if (len > W / 2 - R)
    len = W / 2 - R;
  xx = Xc + co * len;
  yy = Yc + si * len / ratio;

  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, xx - R, yy - R / ratio, xx + R, yy + R / ratio);
} /* End of 'DrawEye' function */

