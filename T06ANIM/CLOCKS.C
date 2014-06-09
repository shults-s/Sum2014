/* FILENAME: CLOCKS.C
 * PROGRAMMER: SS3
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

typedef struct tagss3UNIT_CLOCKS
{
  SS3_UNIT_BASE_FIELDS; /* Включение базовых полей */
  HBITMAP hBm;
} ss3UNIT_CLOCKS;

static VOID ClocksUnitInit( ss3UNIT_CLOCKS *Unit, ss3ANIM *Ani )
{
}

static VOID ClocksUnitClose( ss3UNIT_CLOCKS *Unit, ss3ANIM *Ani )
{
}

static VOID ClocksUnitResponse( ss3UNIT_CLOCKS *Unit, ss3ANIM *Ani )
{
}

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
}

static VOID ClocksUnitRender( ss3UNIT_CLOCKS *Unit, ss3ANIM *Ani)
{
  //               Разрешаем картинке ездить только в переделах экрана
  //               (домножаем, т.к. изначально интервал значений джойстика мал)
  INT W = Ani->W + Ani->JX * Ani->W / 1.5, H = Ani->H + Ani->JY * Ani->H / 1.5;
  SYSTEMTIME st;
  BITMAP bm;
  HDC hDCLogo;
  
  GetObject(Unit->hBm, sizeof(bm), &bm);

  hDCLogo = CreateCompatibleDC(Ani->hDC);
  SelectObject(hDCLogo, Unit->hBm);
  BitBlt(Ani->hDC, W / 2 -175, H / 2 - 175, 350, 350, hDCLogo, 0, 0, SRCCOPY);
  DeleteDC(hDCLogo);

  GetLocalTime(&st);
  DrawArrow(Ani->hDC, W / 2, H / 2, 100, 15, (-(st.wHour % 12 + st.wMinute / 60.0) / 12.0) * 2 * 3.14);
  DrawArrow(Ani->hDC, W / 2, H / 2, 130, 10, (-(st.wMinute + st.wSecond / 60.0) / 60.0) * 2 * 3.14);
  DrawArrow(Ani->hDC, W / 2, H / 2, 150, 3, (-(st.wSecond + st.wMilliseconds / 1000.0) / 60.0) * 2 * 3.14);
}

ss3UNIT * SS3_ClocksUnitCreate( VOID )
{
  ss3UNIT_CLOCKS *Unit;

  if ((Unit = (ss3UNIT_CLOCKS *)SS3_AnimUnitCreate(sizeof(ss3UNIT_CLOCKS))) == NULL)
    return NULL;

  Unit->Init = (VOID *)ClocksUnitInit;
  Unit->Close = (VOID *)ClocksUnitClose;
  Unit->Response = (VOID *)ClocksUnitResponse;
  Unit->Render = (VOID *)ClocksUnitRender;
  Unit->hBm = LoadImage(NULL, "bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  return (ss3UNIT *)Unit;
}