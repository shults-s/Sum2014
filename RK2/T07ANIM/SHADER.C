/* FILENAME: SHADER.C
 * PROGRAMMER: RK2
 * PURPOSE: Shader handle functions.
 * LAST UPDATE: 14.06.2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

/* Loading text file to memory function.
 * ARGUMENTS:
 *   - File name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) - Pointer for loaded file or NULL.
 */
static CHAR *LoadFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT len;

  if ((F = fopen(FileName, "r")) == NULL)
    return NULL;

  /* searching file length */
  fseek(F, 0, SEEK_END);
  len = ftell(F);

  /* Memory alloc */
  if ((txt = malloc(len + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  /* loading file */
  rewind(F);
  len = fread(txt, 1, len, F);
  txt[len] = 0;
  fclose(F);
  return txt;
} /* End of 'LoadFile' function */

/* Shader program initialization function.
 * AERGUMENTS:
 *   - Vertex and pixel shader file names:
 *       CHAR *VSFileName, *FSFileName;
 * RETURNS:
 *   (UINT) - Id of shader program. 0 - if error.
 */
UINT RK2_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName )
{
  CHAR *txt;
  UINT shv, shf, prg;
  INT res, len;
  static CHAR Buf[1000];

  /* Init vertex shader */
  if ((txt = LoadFile(VSFileName)) == NULL)
    return 0;
  if ((shv = glCreateShader(GL_VERTEX_SHADER)) == 0)
  {
    free(txt);
    return 0;
  }
  /* setting text */
  glShaderSource(shv, 1, &txt, NULL);
  free(txt);
  /* shader compile */
  glCompileShader(shv);
  glGetShaderiv(shv, GL_COMPILE_STATUS, &res);
  if (res != 1)
  {
    glGetShaderInfoLog(shv, sizeof(Buf), &len, Buf);
    glDeleteShader(shv);
    return 0;
  }

  /* Pixel shader init */
  if ((txt = LoadFile(FSFileName)) == NULL)
  {
    glDeleteShader(shv);
    return 0;
  }
  if ((shf = glCreateShader(GL_FRAGMENT_SHADER)) == 0)
  {
    glDeleteShader(shv);
    free(txt);
    return 0;
  }
  /* setting text */
  glShaderSource(shf, 1, &txt, NULL);
  free(txt);
  /* shader compile */
  glCompileShader(shf);
  glGetShaderiv(shf, GL_COMPILE_STATUS, &res);
  if (res != 1)
  {
    glGetShaderInfoLog(shf, sizeof(Buf), &len, Buf);
    glDeleteShader(shf);
    glDeleteShader(shv);
    return 0;
  }

  /* Shader program init */
  if ((prg = glCreateProgram()) == 0)
  {
    glDeleteShader(shf);
    glDeleteShader(shv);
    return 0;
  }
  /* Attach shaders to program */
  glAttachShader(prg, shv);
  glAttachShader(prg, shf);

  /* Program linking */
  glLinkProgram(prg);
  glGetProgramiv(prg, GL_LINK_STATUS, &res);
  if (res != 1)
  {
    glGetProgramInfoLog(prg, sizeof(Buf), &len, Buf);
    glDetachShader(prg, shv);
    glDetachShader(prg, shf);
    glDeleteShader(shv);
    glDeleteShader(shf);
    glDeleteProgram(prg);
    return 0;
  }
  return prg;
} /* End of 'RK2_ShadProgInit' function */

/* Removing shader program function.
 * ARGUMENTS:
 *   - Id of loaded program:
 *       UINT ProgId;
 * RETURNS: None.
 */
VOID RK2_ShadProgClose( UINT ProgId )
{
  UINT i, n, shdrs[4];

  if (ProgId == 0)
    return;
  /* Getting attacked shaders */
  glGetAttachedShaders(ProgId, 4, &n, shdrs);

  /* Removing shaders */
  for (i = 0; i < n; i++)
  {
    glDetachShader(ProgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(ProgId);
} /* End of 'RK2_ShadProgClose' function */

/* END OF 'SHADER.C' FILE */
