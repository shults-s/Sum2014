/* FILENAME: MY.C
 * PROGRAMMER: SS3
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* Структура описания объекта анимации */
typedef struct tagss3UNIT_MY
{
  SS3_UNIT_BASE_FIELDS;
  HBITMAP hBm_and, hBm_xor;
} ss3UNIT_MY;

static VOID MyUnitInit( ss3UNIT_MY *Unit, ss3ANIM *Ani )
{
}

static VOID MyUnitClose( ss3UNIT_MY *Unit, ss3ANIM *Ani )
{
}

static VOID MyUnitResponse( ss3UNIT_MY *Unit, ss3ANIM *Ani )
{
}

static VOID MyUnitRender( ss3UNIT_MY *Unit, ss3ANIM *Ani )
{
  //               Разрешаем картинке ездить только в переделах экрана
  //               (домножаем, т.к. изначально интервал значений джойстика мал)
  INT W = Ani->W/* + Ani->JZ * Ani->W / 1.3*/, Y = 0/*Ani->JR * Ani->H / 1.3*/;
  BITMAP bm;
  HDC hScrDC, hMemDC1;

  hScrDC = GetDC(NULL);
  hMemDC1 = CreateCompatibleDC(hScrDC);
  GetObject(Unit->hBm_and, sizeof(bm), &bm);

  if (Y < 0)
    Y = 0;

  if (W > Ani->W)
    W = Ani->W;

  SelectObject(hMemDC1, Unit->hBm_and);
  BitBlt(Ani->hDC, W - bm.bmWidth, Y, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCAND);
  SelectObject(hMemDC1, Unit->hBm_xor);
  BitBlt(Ani->hDC, W - bm.bmWidth, Y, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCINVERT);

  ReleaseDC(NULL, hScrDC);
  DeleteDC(hMemDC1);
}

ss3UNIT * SS3_MyUnitCreate( VOID )
{
  ss3UNIT_MY *Unit;

  if ((Unit = (ss3UNIT_MY *)SS3_AnimUnitCreate(sizeof(ss3UNIT_MY))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)MyUnitInit;
  Unit->Close = (VOID *)MyUnitClose;
  Unit->Response = (VOID *)MyUnitResponse;                                                  
  Unit->Render = (VOID *)MyUnitRender;
  Unit->hBm_and = LoadImage(NULL, "tree_and.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  Unit->hBm_xor = LoadImage(NULL, "tree_xor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  return (ss3UNIT *)Unit;
}

/* Debug info unit */

static VOID InfoUnitRender( ss3UNIT *Unit, ss3ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  TextOut(Ani->hDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
}

ss3UNIT * SS3_InfoUnitCreate( VOID )
{
  ss3UNIT *Unit;

  if ((Unit = SS3_AnimUnitCreate(sizeof(ss3UNIT))) == NULL)
    return NULL;

  /* заполняем поля по-умолчанию */
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
}
/* END OF 'MY.C' FILE */