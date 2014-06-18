


Render
{
  INT i;
  MATR WVP;
  static DBL time;

  /* оси и позиция наблюдателя */
  Ani->MatrWorld = MatrIdentity();
  Ani->MatrView =
    MatrViewLookAt(
      VecMulMatr(VecMulMatr(VecSet(25, 25, 25), MatrRotateY(Ani->JR * 180)), MatrRotateZ(Ani->JY * 180)),
      VecSet(0, 0, 0), VecSet(0, 1, 0));
  WVP = MatrMulMatr(VG4_Anim.MatrWorld, MatrMulMatr(VG4_Anim.MatrView, VG4_Anim.MatrProjection));
  glLoadMatrixf(WVP.A[0]);

  glLineWidth(3);
  glBegin(GL_LINES);
    glColor3d(1, 0.5, 0.5);
    glVertex3d(-3, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0.5, 1, 0.5);
    glVertex3d(0, -3, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0.5, 0.5, 1);
    glVertex3d(0, 0, -3);
    glVertex4d(0, 0, 1, 0);
  glEnd();
  glColorMask(1, 1, 1, 0);
  for (i = -3; i < 30; i++)
  {
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3d(-0.1, -0.1, i);
    glVertex3d(-0.1,  0.1, i);
    glVertex3d( 0.1, -0.1, i);
    glVertex3d( 0.1,  0.1, i);
    glEnd();
  }

  /* Рисуем примитивы */
  time += Ani->GlobalDeltaTime;
  if (time > 1)
  {
    time = 0;
    VG4_ShadProgClose(VG4_ShaderProg);
    VG4_ShaderProg = VG4_ShadProgInit("a.vert", "a.frag");
  }


  glLineWidth(1);
  if (Ani->Keys['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Ani->MatrWorld = MatrMulMatr(MatrRotateX(-90), MatrTranslate(0, 0, 0.30 * sin(Ani->Time)));
  VG4_GeomDraw(&Unit->Obj);
}
