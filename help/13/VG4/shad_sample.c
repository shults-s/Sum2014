  time += Ani->GlobalDeltaTime;
  if (time > 1)
  {
    time = 0;
    VG4_ShadProgClose(VG4_ShaderProg);
    VG4_ShaderProg = VG4_ShadProgInit("a.vert", "a.frag");
  }
 

  /* выбор программы шейдеров вывода примитивов */
  glUseProgram(VG4_ShaderProg);
  loc = glGetUniformLocation(VG4_ShaderProg, "Matr");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.A[0]);
  loc = glGetUniformLocation(VG4_ShaderProg, "Time");
  if (loc != -1)
    glUniform1f(loc, Ani->Time);

