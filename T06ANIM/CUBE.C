/* FILENAME: CUBE.C
 * PROGRAMMER: SS3
 * PURPOSE: 3D unit sample.
 * LAST UPDATE: 10.06.2014
 */

#include "anim.h"
#include "render.h"

typedef struct tagss3UNIT_CUBE
{
  SS3_UNIT_BASE_FIELDS;
  ss3GOBJ Cow;
} ss3UNIT_CUBE;

static VOID CubeUnitInit( ss3UNIT_CUBE *Unit, ss3ANIM *Ani )
{
  SS3_RndGObjLoad(&Unit->Cow, "porshe.object");
}

static VOID CubeUnitClose( ss3UNIT_CUBE *Unit, ss3ANIM *Ani )
{
  SS3_RndGObjFree(&Unit->Cow);
}

static VOID CubeUnitResponse( ss3UNIT_CUBE *Unit, ss3ANIM *Ani )
{
  SS3_RndWs = Ani->W;
  SS3_RndHs = Ani->H;
  SS3_RndWp = SS3_RndHp * Ani->W / Ani->H;
  SS3_RndMatrProjection = MatrProjection(-SS3_RndWp / 2, SS3_RndWp / 2, -SS3_RndHp / 2, SS3_RndHp / 2, SS3_RndProjDist, 10000.0);
}

static VOID CubeUnitRender( ss3UNIT_CUBE *Unit, ss3ANIM *Ani )
{
  static INT Direct = 1;
  DBL Scale = Ani->JX / 10;

  SS3_RndMatrView = MatrViewLookAt(VecMulMatr(VecSet(0, 0, 1), MatrRotateX(-15 + 30 * Ani->JY)), VecSet(0, 0, 0), VecSet(0, 1, 0));

  if (Ani->JButs[4])
    Direct = -1;
  else if (Ani->JButs[5])
    Direct = 1;

  SS3_RndMatrWorld = MatrRotateY(Ani->Time * Direct * 20);
  SS3_RndMatrWorld = MatrMulMatr(SS3_RndMatrWorld, MatrScale(0.30 + Scale, 0.30 + Scale, 0.30 + Scale));

  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));

  SS3_RndGObjDraw(&Unit->Cow, Ani->hDC);
  /*INT i, s = 5, N = 30 / 30;
  VEC p = {1, 0, 0};
  POINT pt;
  static DBL Delta = 0.1;

  Delta += Ani->JZ * Ani->GlobalDeltaTime;

  SS3_RndMatrView = MatrViewLookAt(VecMulMatr(VecSet(0, 0, Ani->JX * Delta + 15), MatrRotateX(90 * Ani->JY)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  SS3_RndMatrWorld = MatrRotateY(Ani->Time * 30);
  SS3_RndMatrWorld = MatrMulMatr(SS3_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));

  SetDCBrushColor(Ani->hDC, RGB(255, 255, 255));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));

  SS3_RndGObjDraw(&Unit->Cow, Ani->hDC);

  SS3_RndMatrWorld = MatrRotateY(Ani->Time * 30);

  SS3_RndMatrWorld = MatrMulMatr(SS3_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));
  SS3_RndMatrWorld = MatrMulMatr(MatrTranslate(0.0, 0.0, 3 * 3.30), SS3_RndMatrWorld);
  SS3_RndGObjDraw(&Unit->Cow, Ani->hDC);*/
}

ss3UNIT * SS3_CubeUnitCreate( VOID )
{
  ss3UNIT_CUBE *Unit;

  if ((Unit = (ss3UNIT_CUBE *)SS3_AnimUnitCreate(sizeof(ss3UNIT_CUBE))) == NULL)
    return NULL;

  Unit->Init = (VOID *)CubeUnitInit;
  Unit->Close = (VOID *)CubeUnitClose;
  Unit->Response = (VOID *)CubeUnitResponse;
  Unit->Render = (VOID *)CubeUnitRender;
  return (ss3UNIT *)Unit;
}