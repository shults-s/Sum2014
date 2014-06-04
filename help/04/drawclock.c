/* Функция рисования стрелки.
 * АРГУМЕНТЫ:
 *   - контекст:
 *       HDC hDC;
 *   - координаты центра:
 *       INT Xc, Yc;
 *   - длина и ширина стрелки:
 *       INT L, W;
 *   - угол рисования:
 *       FLOAT Angle;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, FLOAT Angle )
{
  INT i;
  POINT pts[] =
  {
    {0, -W}, {-W, 0}, {0, L}, {W, 0}
  }, pts_draw[sizeof pts / sizeof pts[0]];
  FLOAT si = sin(Angle), co = cos(Angle);

  for (i = 0; i < sizeof pts / sizeof pts[0]; i++)
  {
    pts_draw[i].x = Xc + (pts[i].x * co - pts[i].y * si);
    pts_draw[i].y = Yc - (pts[i].x * si + pts[i].y * co);
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Polygon(hDC, pts_draw, sizeof pts / sizeof pts[0]);
} /* End of 'DrawArrow' function */

----------------------------------------------------
  SYSTEMTIME st;
  FLOAT PI = 3.14159265358979323846;

    GetLocalTime(&st);

    DrawArrow(hMemDC, W / 2, H / 2, 50, 20, (-(st.wHour % 12 + st.wMinute / 60.0) / 12.0) * 2 * PI);
    DrawArrow(hMemDC, W / 2, H / 2, 80, 15, (-(st.wMinute + st.wSecond / 60.0) / 60.0) * 2 * PI);
    DrawArrow(hMemDC, W / 2, H / 2, 90, 10, (-(st.wSecond + st.wMilliseconds / 1000.0) / 60.0) * 2 * PI);
    DrawArrow(hMemDC, W / 2, H / 2, 100, 3, (-st.wMilliseconds / 1000.0) * 2 * PI);
