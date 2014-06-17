/* FILENAME: UNITGOBJ.C
 * PROGRAMMER: SS3
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 13.06.2014
 */

#include <string.h>
#include <stdlib.h>

#include "anim.h"

typedef struct tagss3UNIT_GOBJ
{
  SS3_UNIT_BASE_FIELDS;
  ss3GEOM *Obj;
} ss3UNIT_GOBJ;

ss3CAMERA SS3_RndCam;

static VOID SS3_GobjUnitInit( ss3UNIT_GOBJ *Unit, ss3ANIM *Ani )
{
  if ((Unit->Obj = malloc(sizeof(ss3GEOM))) == NULL)
    return;

  SS3_GeomLoad(&Unit->Obj[0], "./Su27/Su27(2).object");
}

static VOID SS3_GobjUnitClose( ss3UNIT_GOBJ *Unit, ss3ANIM *Ani )
{
  SS3_GeomFree(&Unit->Obj[0]);
}

static VOID SS3_GobjUnitResponse( ss3UNIT_GOBJ *Unit, ss3ANIM *Ani )
{
}

static VOID SS3_GobjUnitRender( ss3UNIT_GOBJ *Unit, ss3ANIM *Ani )
{
  MATR WVP;
  static DBL time;

  /* оси и позиция наблюдателя */
  Ani->MatrWorld = MatrIdentity();

  /*Ani->MatrView =
    MatrViewLookAt(
    MatrMultVec(MatrRotateY(Ani->JR * 90), MatrMultVec(MatrRotateZ(Ani->JY * 90), Ani->PosCam)),
      VecSet(0, 0, 0), VecSet(0, 1, 0));
  WVP = MatrMult4x4(SS3_Anim.MatrWorld, MatrMult4x4(SS3_Anim.MatrView, SS3_Anim.MatrProjection));*/

  Ani->MatrView =
    MatrViewLookAt(
      VecMulMatr(VecSet(Ani->PosCam.X, Ani->PosCam.Y, Ani->PosCam.Z), MatrRotateZ(-30 + Ani->JY * 90)), VecSet(0, 0, 0), VecSet(0, 1, 0));

  WVP = MatrMulMatr(SS3_Anim.MatrWorld, MatrMulMatr(SS3_Anim.MatrView, SS3_Anim.MatrProjection));
  glLoadMatrixf(WVP.A[0]);

  /* Рисуем примитивы */
  time += Ani->GlobalDeltaTime;
  if (time > 1)
  {
    time = 0;
    SS3_ShadProgClose(SS3_ShaderProg);
    SS3_ShaderProg = SS3_ShadProgInit("a.vert", "a.frag");
  }

  glLineWidth(1);
  if (Ani->Keys['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Ani->MatrWorld = MatrRotateY(Ani->Time * 30);

  glEnable(GL_DEPTH_TEST);
  SS3_GeomDraw(&Unit->Obj[0]);
  glUseProgram(0);
}

ss3UNIT * SS3_GobjUnitCreate( VOID )
{
  ss3UNIT_GOBJ *Unit;

  if ((Unit = (ss3UNIT_GOBJ *)SS3_AnimUnitCreate(sizeof(ss3UNIT_GOBJ))) == NULL)
    return NULL;

  Unit->Init = (VOID *)SS3_GobjUnitInit;
  Unit->Close = (VOID *)SS3_GobjUnitClose;
  Unit->Response = (VOID *)SS3_GobjUnitResponse;
  Unit->Render = (VOID *)SS3_GobjUnitRender;
  return (ss3UNIT *)Unit;
}