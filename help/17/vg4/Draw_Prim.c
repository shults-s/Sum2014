/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - примитив:
 *       vg4PRIM *P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_PrimDraw( vg4PRIM *P )
{
  INT i;

  if (P->V == NULL)
    return;

  if (P->Buffers[0] == 0)
  {
    /* создаем и заполняем буфера данных */
    glGenBuffers(2, P->Buffers);
    glGenVertexArrays(1, &P->VertexBuffer);

    /* вершины */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vg4VERTEX) * P->NumOfV,
      P->V, GL_STATIC_DRAW);

    /* индексы */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(INT) * P->NumOfI,
      P->I, GL_STATIC_DRAW);
  }

  /* активируем буфера */
  glBindVertexArray(P->VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, P->Buffers[1]);

  /* задаем порядок данных */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(vg4VERTEX), (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(vg4VERTEX), (VOID *)(sizeof(VEC)));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(vg4VERTEX), (VOID *)(sizeof(VEC) + sizeof(vg4UV)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(vg4VERTEX), (VOID *)(2 * sizeof(VEC) + sizeof(vg4UV)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glUseProgram(VG4_ShaderProg);
  if (P->Type == VG4_PRIM_TRIMESH)
  {
    /*
    glBegin(GL_TRIANGLES);
    for (i = 0; i < P->NumOfI; i++)
      glVertex3fv(&P->V[P->I[i]].P.X);
    glEnd();
    */
    glDrawElements(GL_TRIANGLES, P->NumOfI, GL_UNSIGNED_INT, (VOID *)0);
  }
  else
  {
    for (i = 0; i < P->GH - 1; i++)
    {
      /*
      INT start = i * P->GW, j;

      glBegin(GL_TRIANGLE_STRIP);
      for (j = 0; j < P->NumOfI; j++)
        glVertex3fv(&P->V[start + P->I[j]].P.X);
      glEnd();
      */
      /*
      glDrawElements(GL_TRIANGLE_STRIP, P->NumOfI, GL_UNSIGNED_INT,
        (VOID *)(i * P->GW * 2 * sizeof(INT)));
          glDrawElements(GL_TRIANGLES, P->NumOfI, GL_UNSIGNED_INT, (VOID *)0);
      */
      glDrawElementsBaseVertex(GL_TRIANGLE_STRIP,
        P->NumOfI, GL_UNSIGNED_INT, (VOID *)0,
        i * P->GW);

    }
  }
  glUseProgram(0);
} /* End of 'VG4_PrimFree' function */
