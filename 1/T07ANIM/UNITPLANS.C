/* FILENAME: UNITPLANS.C
 * PROGRAMMER: EF2
 * PURPOSE: Plans unit handle module.
 * LAST UPDATE: 17.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* Main type of unit */
typedef struct tagef2UNIT_PLANS
{
  EF2_UNIT_BASE_FIELDS;  /* Base fields of unit */
  ef2GEOM Obj;           /* Geometric object */
  INT SizeCube;
} ef2UNIT_PLANS;

ef2CAMERA EF2_RndCam;

/* Init skyblock unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT_PLANS *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID EF2_PlansUnitInit( ef2UNIT_PLANS *Unit, ef2ANIM *Ani )
{
  ef2PRIM Pr[6] = {0};
  ef2MATERIAL Mtl[6] = {0};
  INT a = 100;

  Unit->SizeCube = a;
  memset(&Unit->Obj, 0, sizeof(ef2GEOM));
  Mtl[0].Trans = 1;
  Mtl[0].Phong = 30;
  Mtl[0].Ka = VecSet(0.1, 0.1, 0.1);
  Mtl[0].Kd = VecSet(1, 1, 1);
  Mtl[0].Ks = VecSet(0, 0, 0);
  strcpy(Mtl[0].MapD, "./Textures/sky_bot1.bmp");
  strcpy(Mtl[0].Name, "Sky Box Down Field Material");

  
  Mtl[1].Trans = 1;
  Mtl[1].Phong = 30;
  Mtl[1].Ka = VecSet(0.1, 0.1, 0.1);
  Mtl[1].Kd = VecSet(1, 1, 1);
  Mtl[1].Ks = VecSet(0, 0, 0);
  strcpy(Mtl[1].MapD, "./Textures/sky_dir1.bmp");
  strcpy(Mtl[1].Name, "Sky Box Dir Field Material");
  /* Right */
  Mtl[2].Trans = 1;
  Mtl[2].Phong = 30;
  Mtl[2].Ka = VecSet(0.1, 0.1, 0.1);
  Mtl[2].Kd = VecSet(1, 1, 1);
  Mtl[2].Ks = VecSet(0, 0, 0);
  strcpy(Mtl[2].MapD, "./Textures/sky_right1.bmp");
  strcpy(Mtl[2].Name, "Sky Box Right Field Material");
  /* Down */
  Mtl[3].Trans = 1;
  Mtl[3].Phong = 30;
  Mtl[3].Ka = VecSet(0.1, 0.1, 0.1);
  Mtl[3].Kd = VecSet(1, 1, 1);
  Mtl[3].Ks = VecSet(0, 0, 0);
  strcpy(Mtl[3].MapD, "./Textures/sky_down1.bmp");
  strcpy(Mtl[3].Name, "Sky Box Back Field Material");

  /* left */
  Mtl[4].Trans = 1;
  Mtl[4].Phong = 30;
  Mtl[4].Ka = VecSet(0.1, 0.1, 0.1);
  Mtl[4].Kd = VecSet(1, 1, 1);
  Mtl[4].Ks = VecSet(0, 0, 0);
  strcpy(Mtl[4].MapD, "./Textures/sky_left1.bmp");
  strcpy(Mtl[4].Name, "Sky Box Left Field Material");

  /* Up */
  Mtl[5].Trans = 1;
  Mtl[5].Phong = 30;
  Mtl[5].Ka = VecSet(0.1, 0.1, 0.1);
  Mtl[5].Kd = VecSet(1, 1, 1);
  Mtl[5].Ks = VecSet(0, 0, 0);
  strcpy(Mtl[5].MapD, "./Textures/sky_up1.bmp");
  strcpy(Mtl[5].Name, "Sky Box Up Field Material");  
 
  /* Create planes */

  EF2_PrimCreatePlane(&Pr[0], VecSet(-a, 0, a), VecSet(a, 0, a), VecSet(-a, 0, -a), VecSet(a, 0, -a), VecSet(0, 1, 0), 1);
  Pr[0].Mtl = EF2_GeomAddMaterial(&Unit->Obj, &Mtl[0]);;
  EF2_GeomAddPrim(&Unit->Obj, &Pr[0]);

  EF2_PrimCreatePlane(&Pr[1], VecSet(-a, 0, -a), VecSet(a, 0, -a), VecSet(-a, a, -a), VecSet(a, a, -a), VecSet(0, 0, 1), 1);
  Pr[1].Mtl = EF2_GeomAddMaterial(&Unit->Obj, &Mtl[1]);
  EF2_GeomAddPrim(&Unit->Obj, &Pr[1]);
  
  EF2_PrimCreatePlane(&Pr[2],VecSet(-a, 0, a), VecSet(-a, 0, -a), VecSet(-a, a, a), VecSet(-a, a, -a), VecSet(1, 0, 0), 1);
  Pr[2].Mtl = EF2_GeomAddMaterial(&Unit->Obj, &Mtl[4]);
  EF2_GeomAddPrim(&Unit->Obj, &Pr[2]);

  EF2_PrimCreatePlane(&Pr[3], VecSet(a, 0, a), VecSet(-a, 0, a), VecSet(a, a, a), VecSet(-a, a, a), VecSet(0, 0, -1), 1);
  Pr[3].Mtl = EF2_GeomAddMaterial(&Unit->Obj, &Mtl[3]);
  EF2_GeomAddPrim(&Unit->Obj, &Pr[3]);

  EF2_PrimCreatePlane(&Pr[4], VecSet(a, 0, -a), VecSet(a, 0, a), VecSet(a, a, -a), VecSet(a, a, a), VecSet(-1, 0, 0), 1);
  Pr[4].Mtl = EF2_GeomAddMaterial(&Unit->Obj, &Mtl[2]);
  EF2_GeomAddPrim(&Unit->Obj, &Pr[4]);

  EF2_PrimCreatePlane(&Pr[5],  VecSet(-a, a, -a), VecSet(a, a, -a), VecSet(-a, a, a), VecSet(a, a, a), VecSet(0, -1, 0), 1);
  Pr[5].Mtl = EF2_GeomAddMaterial(&Unit->Obj, &Mtl[5]);
  EF2_GeomAddPrim(&Unit->Obj, &Pr[5]);

} /* End of 'EF2_AnimUnitInit' function */

/* Close skyblock unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT_PLANS *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID EF2_PlansUnitClose( ef2UNIT_PLANS *Unit, ef2ANIM *Ani )
{
  EF2_GeomFree(&Unit->Obj);
} /* End of 'EF2_AnimUnitClose' function */

/* Response unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT_PLANS *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID EF2_PlansUnitResponse( ef2UNIT_PLANS *Unit, ef2ANIM *Ani )
{
} /* End of 'EF2_AnimUnitResponse' function */

/* Render skyblock unit of animation function.
 * ARGUMENTS:
 *   - pointer for animation:
 *       ef2UNIT_PLANS *Unit;
 *   - pointer to animation:
 *       ef2ANIM *Ani;
 * RETURNS: None.
 */
static VOID EF2_PlansUnitRender( ef2UNIT_PLANS *Unit, ef2ANIM *Ani )
{
  MATR WVP;
  static DBL time;
  /* оси и позиция наблюдателя */
  EF2_Anim.MatrWorld = MatrIdenity();
  //EF2_Anim.MatrWorld = MatrTranslate(((INT)EF2_RndCam.Loc.X % Unit->SizeCube) * Unit->SizeCube, ((INT)EF2_RndCam.Loc.Y % Unit->SizeCube) * Unit->SizeCube, 
  //  ((INT)EF2_RndCam.Loc.Z % Unit->SizeCube) * Unit->SizeCube);
  //EF2_Anim.MatrWorld = MatrTranslate(EF2_RndCam.Loc.X, EF2_RndCam.Loc.Y - Unit->SizeCube / 2, EF2_RndCam.Loc.Z);
  WVP = EF2_MatrMult4x4(EF2_Anim.MatrWorld, EF2_MatrMult4x4(EF2_Anim.MatrView, EF2_Anim.MatrProjection));
  glLoadMatrixf(WVP.A[0]);

  if (Ani->Keys['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);
  EF2_GeomDraw(&Unit->Obj);
  glUseProgram(0);
  EF2_Anim.MatrWorld = MatrIdenity();
} /* End of 'EF2_AnimUnitRender' function */

/* Create skyblock unit of animation function.
 * ARGUMENTS:
 *   - size to structure of animation:
 *       INT Size;
 * RETURNS:
 *   (ef2UNIT_PLANS *) pointer to unit.
 */
ef2UNIT * EF2_PlansUnitCreate( VOID )
{
  ef2UNIT_PLANS *Unit;

  if ((Unit = (ef2UNIT_PLANS *)EF2_AnimUnitCreate(sizeof(ef2UNIT_PLANS))) == NULL)
    return NULL;
  /* create default unit */
  Unit->Init = (VOID *)EF2_PlansUnitInit;
  Unit->Close = (VOID *)EF2_PlansUnitClose;
  Unit->Response = (VOID *)EF2_PlansUnitResponse;
  Unit->Render = (VOID *)EF2_PlansUnitRender;
  return (ef2UNIT *)Unit;
} /* End of 'EF2_AnimUnitCreate' function */
