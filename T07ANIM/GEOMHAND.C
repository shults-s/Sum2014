/* FILENAME: GEOMHAND.C
 * PROGRAMMER: SS3
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 14.06.2014
 */

#include <stdlib.h>

#include "anim.h"

/* Функция добавления материала к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ss3GEOM *G;
 *   - добавляемый материал:
 *       ss3MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного материала или -1
 *         при ошибке.
 */
INT SS3_GeomAddMaterial( ss3GEOM *G, ss3MATERIAL *Mtl )
{
  INT i;
  ss3MATERIAL *M;

  /* ищем материал в библиотеке */
  for (i = 0; i < G->NumOfMtls; i++)
    if (strcmp(G->Mtls[i].Name, Mtl->Name)== 0)
    {
      G->Mtls[i] = *Mtl;
      return i;
    }

  if ((M = malloc(sizeof(ss3MATERIAL) * (G->NumOfMtls + 1))) == NULL)
    return -1;
  if (G->Mtls != NULL)
  {
    /* копируем старые данные */
    memcpy(M, G->Mtls, sizeof(ss3MATERIAL) * G->NumOfMtls);
    /* освобождаем память */
    free(G->Mtls);
  }
  /* добавляем */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'SS3_GeomAddMaterial' function */

/* Функция добавления примитива к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ss3GEOM *G;
 *   - добавляемый примитив:
 *       ss3PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива или -1
 *         при ошибке.
 */
INT SS3_GeomAddPrim( ss3GEOM *G, ss3PRIM *Prim )
{
  ss3PRIM *P;

  if ((P = malloc(sizeof(ss3PRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* копируем старые данные */
    memcpy(P, G->Prims, sizeof(ss3PRIM) * G->NumOfPrims);
    /* освобождаем память */
    free(G->Prims);
  }
  /* добавляем */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'SS3_GeomAddPrim' function */

/* Функция освобождения */
VOID SS3_GeomFree( ss3GEOM *G )
{
  INT i;

  for (i = 0; i < G->NumOfPrims; i++)
    SS3_PrimFree(G->Prims + i);
  free(G->Prims);
  for (i = 0; i < G->NumOfMtls; i++)
    glDeleteTextures(1, &G->Mtls[i].TexNo);
  free(G->Mtls);
  memset(G, 0, sizeof(ss3GEOM));
} /* End of 'SS3_GeomFree' function */

/* Функция рисования */
VOID SS3_GeomDraw( ss3GEOM *G )
{
  INT i, loc;
  MATR WVP, MatrWorldInvTrans;
  VEC V;

  /* вычислили матрицы преобразования */
  WVP = MatrMult4x4(SS3_Anim.MatrWorld,
    MatrMult4x4(SS3_Anim.MatrView, SS3_Anim.MatrProjection));
  MatrWorldInvTrans = MatrTranspose(MatrInverse(SS3_Anim.MatrWorld));

  /* выбор программы шейдеров вывода примитивов */
  glUseProgram(SS3_ShaderProg);
  loc = glGetUniformLocation(SS3_ShaderProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.A[0]);

  loc = glGetUniformLocation(SS3_ShaderProg, "MatrWorldInverseTranspose");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MatrWorldInvTrans.A[0]);

  loc = glGetUniformLocation(SS3_ShaderProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, SS3_Anim.MatrWorld.A[0]);

  loc = glGetUniformLocation(SS3_ShaderProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, SS3_Anim.MatrView.A[0]);

  loc = glGetUniformLocation(SS3_ShaderProg, "Time");
  if (loc != -1)
    glUniform1f(loc, SS3_Anim.Time);

  V = VecSet(-SS3_Anim.MatrView.A[0][2],
             -SS3_Anim.MatrView.A[1][2],
             -SS3_Anim.MatrView.A[2][2]);
  loc = glGetUniformLocation(SS3_ShaderProg, "ViewDir");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);


  for (i = 0; i < G->NumOfPrims; i++)
  {
    INT mtl = G->Prims[i].Mtl;

    /* подготавливаем материал */
    if (G->Mtls != NULL)
    {
      INT loc;

      if (G->Mtls[mtl].TexNo == 0 && G->Mtls[mtl].MapD[0] != 0)
      {
        INT j;
        IMAGE Img;
                       
        ImageLoad(&Img, G->Mtls[mtl].MapD);

        /* получаем свободный номер текстуры */
        glGenTextures(1, &G->Mtls[mtl].TexNo);
        /* делаем ее активной */
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        for (j = 0; j < Img.W * Img.H; j++)
          Img.Bits[j] |= 0xFF000000;
        /* отправляем картинку в видеопамять */
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

        /* Параметры вывода */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      /* передаем параметры */
      if (G->Mtls[mtl].TexNo != 0)
      {
        loc = glGetUniformLocation(SS3_ShaderProg, "DrawTexture");
        if (loc != -1)
          glUniform1i(loc, 0);
        /*glEnable(GL_TEXTURE_2D);*/
        /*glActiveTexture(GL_TEXTURE0);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        /*glActiveTexture(GL_TEXTURE1);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
      }
      loc = glGetUniformLocation(SS3_ShaderProg, "Ka");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ka.X);
      loc = glGetUniformLocation(SS3_ShaderProg, "Kd");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Kd.X);
      loc = glGetUniformLocation(SS3_ShaderProg, "Ks");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ks.X);
      loc = glGetUniformLocation(SS3_ShaderProg, "Phong");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Phong);
      loc = glGetUniformLocation(SS3_ShaderProg, "Trans");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Trans);
    }
    SS3_PrimDraw(G->Prims + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
} /* End of 'SS3_GeomDraw' function */

/* END OF 'GEOMHAND.C' FILE */
