Unit:
  GEOM Obj;
--------------------
  vg4PRIM prim = {0};
  vg4MATERIAL mtl = {0};

  memset(&Unit->Obj, 0, sizeof(vg4GEOM));

  VG4_PrimCreateHeightField(&prim, "hf.bmp", 30, 10);

  mtl.Ka = VecSet(0.1, 0.1, 0.1);
  mtl.Kd = VecSet(1, 1, 1);
  mtl.Ks = VecSet(0, 0, 0);
  mtl.Phong = 30;
  mtl.Trans = 1;
  mtl.TexNo = 0;
  mtl.MapD[0] = 0;
  strcpy(mtl.MapD, "map2-hf.bmp");
  strcpy(mtl.Name, "Height Field Material");
  prim.Mtl = VG4_GeomAddMaterial(&Unit->Obj, &mtl);

  VG4_GeomAddPrim(&Unit->Obj, &prim);

. . .

  Ani->MatrWorld = MatrIdentity();
  Ani->MatrView =
    MatrViewLookAt(
      VecMulMatr(VecMulMatr(VecSet(25, 25, 25), MatrRotateY(Ani->JR * 180)), MatrRotateZ(Ani->JY * 180)),
      VecSet(0, 0, 0), VecSet(0, 1, 0));
  WVP = MatrMulMatr(VG4_Anim.MatrWorld,
    MatrMulMatr(VG4_Anim.MatrView, VG4_Anim.MatrProjection));
  glLoadMatrixf(WVP.A[0]);
. . .
  Ani->MatrWorld = MatrMulMatr(MatrRotateX(-0), MatrTranslate(0, 0, 0.30 * sin(Ani->Time)));
  VG4_GeomDraw(&Unit->Obj);



