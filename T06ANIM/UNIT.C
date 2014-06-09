/* FILENAME: UNIT.C
 * PROGRAMMER: SS3
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ss3UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ss3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID SS3_AnimUnitInit( ss3UNIT *Unit, ss3ANIM *Ani )
{
} /* End of 'SS3_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ss3UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ss3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID SS3_AnimUnitClose( ss3UNIT *Unit, ss3ANIM *Ani )
{
} /* End of 'SS3_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ss3UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ss3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID SS3_AnimUnitResponse( ss3UNIT *Unit, ss3ANIM *Ani )
{
} /* End of 'SS3_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ss3UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ss3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID SS3_AnimUnitRender( ss3UNIT *Unit, ss3ANIM *Ani )
{
} /* End of 'SS3_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ss3UNIT *) указатель на созданный объект анимации.
 */
ss3UNIT * SS3_AnimUnitCreate( INT Size )
{
  ss3UNIT *Unit;

  if (Size < sizeof(ss3UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* заполняем поля по-умолчанию */
  Unit->Size = Size;
  Unit->Init = SS3_AnimUnitInit;
  Unit->Close = SS3_AnimUnitClose;
  Unit->Response = SS3_AnimUnitResponse;
  Unit->Render = SS3_AnimUnitRender;
  return Unit;
} /* End of 'SS3_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

