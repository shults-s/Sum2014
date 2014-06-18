  if (TexNo == 0)
  {
    IMAGE Img;
                   
    ImageLoad(&Img, "m.bmp");

    /* получаем свободный номер текстуры */
    glGenTextures(1, &TexNo);
    /* делаем ее активной */
    glBindTexture(GL_TEXTURE_2D, TexNo);
    for (i = 0; i < Img.W * Img.H; i++)
      Img.Bits[i] |= 0xFF000000;
    /* отправляем картинку в видеопамять */
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
      GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

    /* Параметры вывода */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, TexNo);
  glColor3d(1, 1, 1);
  glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2d(0, 1);
    glVertex3d(0, 1, 0);
    glTexCoord2d(0, 0);
    glVertex3d(0, 0, 0);
    glTexCoord2d(1, 1 + sin(Ani->Time));
    glVertex3d(1, 1, 0);
    glTexCoord2d(1, 0);
    glVertex3d(1, 0, 0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

