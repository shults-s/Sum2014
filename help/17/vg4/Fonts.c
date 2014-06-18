Init:
  wglUseFontBitmaps(Ani->hDC, 0, 256, 111);

Render:

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);
  Ani->MatrWorld = MatrIdentity();
  glLoadMatrixf(Ani->MatrWorld.A[0]);
  glColor3d(1, 1, 1);
  glRasterPos3d(-1, 0.9, 0); 
  glListBase(111);
  glCallLists(sprintf(Buf, "FPS: %.5f", Ani->FPS), GL_UNSIGNED_BYTE, Buf);
  glPopAttrib();;
