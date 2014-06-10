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
}

static VOID CubeUnitRender( ss3UNIT_CUBE *Unit, ss3ANIM *Ani )
{
  SS3_RndMatrView = MatrViewLookAt(VecMulMatr(VecSet(0, 0, Ani->JX * 5 + 15), MatrRotateX(30 * Ani->JY)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  SS3_RndMatrWorld = MatrRotateY(Ani->Time * 20);
  SS3_RndMatrWorld = MatrMulMatr(SS3_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));

  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));
  SS3_RndGObjDraw(&Unit->Cow, Ani->hDC);
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