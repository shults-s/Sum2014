 /* FILENAME: GOBJ.C
 * PROGRAMMER: MY6
 * PURPOSE: GObj Unit
 * LAST UPDATE: 17.06.2014
 */

#include "anim.h"
#include "render.h"
#include "geom.h"

/* Структура описания объекта анимации */
typedef struct tagmy6UNIT_GOBJ
{
  MY6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  my6GEOM Road;
  my6GEOM Car;
} my6UNIT_GOBJ;

MATR WVP;

FLT RoadR = 20000;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID GObjUnitInit( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  my6PRIM prim = {0};
  my6MATERIAL mtl = {0};

  memset(&Unit->Road, 0, sizeof(my6GEOM));
  memset(&Unit->Car, 0, sizeof(my6GEOM));

  MY6_GeomLoad(&Unit->Car, "e:\\Models\\x6\\x6.object");
  MY6_GeomTransform(&Unit->Car, MatrRotateX(-90));
  MY6_GeomTransform(&Unit->Car, MatrRotateY(180));
  MY6_GeomTransform(&Unit->Car, MatrTranslate(1.5, 0.87, 0));


  MY6_PrimCreateCylinder(&prim, 200, RoadR, 2000, 5);

  mtl.Ka = VecSet(0.1, 0.1, 0.1);
  mtl.Kd = VecSet(1, 1, 1);
  mtl.Ks = VecSet(0, 0, 0);
  mtl.Phong = 30;
  mtl.Trans = 1;
  mtl.TexNo = 0;
  mtl.MapD[0] = 0;
  strcpy(mtl.MapD, "r.bmp");
  strcpy(mtl.Name, "Road material");
  prim.Mtl = MY6_GeomAddMaterial(&Unit->Road, &mtl);

  MY6_GeomAddPrim(&Unit->Road, &prim);

} /* End of 'GObjUnitInit' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID GObjUnitResponse( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
} /* End of 'GObjUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID GObjUnitRender( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  INT loc;
  static INT ShadProgId;
  static DBL rotatey, rotatex, scale, tr, time, full_shift = 0, vert_shift, hor_shift;

  rotatey += Ani->JX * 4;
  rotatex += Ani->JY * 4;
  scale += Ani->JR / 50;
  tr += Ani->JZ * 10;

  vert_shift += (Ani->JPOV == 1) * Ani->DeltaTime * 3.0 - (Ani->JPOV == 5) * Ani->DeltaTime * 3.0;
  hor_shift += (Ani->JPOV == 7) * Ani->DeltaTime * 3.0 - (Ani->JPOV == 3) * Ani->DeltaTime * 3.0;

  Ani->MatrView = MatrViewLookAt(VecSet(0, 5 + vert_shift, 30), VecSet(0, 1, 0), VecSet(0, 1, 0));

  Ani->MatrProjection = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                       -Ani->Hp / 2, Ani->Hp / 2,
                                        Ani->ProjDist, Ani->FarClip);

  Ani->MatrWorld = MatrScale(1 + scale, 1 + scale, 1 + scale);
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrRotateY(rotatey));
  Ani->MatrWorld = MatrMulMatr(Ani->MatrWorld, MatrRotateX(rotatex));

  WVP = MatrMulMatr(Ani->MatrWorld, MatrMulMatr(Ani->MatrView, Ani->MatrProjection));

  glLoadMatrixf(WVP.A[0]);

  if (Ani->JButs[5])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  time += Ani->GlobalDeltaTime;
  if (time > 1)
  {
    time = 0;
    MY6_ShadProgClose(MY6_ShaderProg);
    MY6_ShaderProg = MY6_ShadProgInit("a.vert", "a.frag");
  }

  glUseProgram(MY6_ShaderProg);

  loc = glGetUniformLocation(MY6_ShaderProg, "J0");
  if (loc != -1)
    glUniform1f(loc, Ani->JButs[0]);
  loc = glGetUniformLocation(MY6_ShaderProg, "J1");
  if (loc != -1)
    glUniform1f(loc, Ani->JButs[1]);

  Ani->MatrWorld = MatrIdentity();
  MY6_GeomDraw(&Unit->Car);

  full_shift += Ani->DeltaTime * 3.0 * Ani->JZ;
  Ani->MatrWorld = MatrMulMatr(MatrRotateX(full_shift), MatrTranslate(hor_shift, -RoadR, 0));
  MY6_GeomDraw(&Unit->Road);
  glUseProgram(0);
} /* End of 'GObjUnitRender' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       my6UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       my6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitClose( my6UNIT_GOBJ *Unit, my6ANIM *Ani )
{
  MY6_GeomFree(&Unit->Road);
} /* End of 'GObjUnitClose' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (my6UNIT *) указатель на созданный объект анимации.
 */

my6UNIT * MY6_GObjUnitCreate( VOID )
{
  my6UNIT_GOBJ *Unit;

  if ((Unit = (my6UNIT_GOBJ *)MY6_AnimUnitCreate(sizeof(my6UNIT_GOBJ))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)GObjUnitInit;
  Unit->Close = (VOID *)GObjUnitClose;
  Unit->Response = (VOID *)GObjUnitResponse;
  Unit->Render = (VOID *)GObjUnitRender;
  return (my6UNIT *)Unit;
} /* End of 'MY6_GObjUnitCreate' function */

/* END OF 'UNITGOBJ.C' FILE */
