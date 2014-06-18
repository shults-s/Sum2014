/* FILENAME: UNITINFO.C
 * PROGRAMMER: VG4
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* Структура описания объекта анимации */
typedef struct tagvg4UNIT_INFO
{
  VG4_UNIT_BASE_FIELDS; /* Включение базовых полей */
} vg4UNIT_INFO;

/* Функция инициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitInit( vg4UNIT_INFO *Unit, vg4ANIM *Ani )
{
} /* End of 'InfoUnitInit' function */

/* Функция деинициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitClose( vg4UNIT_INFO *Unit, vg4ANIM *Ani )
{
} /* End of 'InfoUnitClose' function */

/* Функция обновления межкадровых параметров информационного
 * объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitResponse( vg4UNIT_INFO *Unit, vg4ANIM *Ani )
{
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    VG4_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    VG4_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
} /* End of 'InfoUnitResponse' function */

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT *Unit;
 *   - указатель на контекст анимации:
 *       vg4UNIT_INFO *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitRender( vg4UNIT_INFO *Unit, vg4ANIM *Ani )
{
  INT x, y;
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);
} /* End of 'InfoUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UNIT_INFO *) указатель на созданный объект анимации.
 */
vg4UNIT * VG4_InfoUnitCreate( VOID )
{
  vg4UNIT_INFO *Unit;

  if ((Unit = (vg4UNIT_INFO *)VG4_AnimUnitCreate(sizeof(vg4UNIT_INFO))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Render = (VOID *)InfoUnitRender;
  Unit->Response = (VOID *)InfoUnitResponse;
  return (vg4UNIT *)Unit;
} /* End of 'VG4_InfoUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
