/* FILENAME: UNITINFO.C
 * PROGRAMMER: SS3
 * PURPOSE: Information unit handle module.
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

typedef struct tagss3UNIT_INFO
{
  SS3_UNIT_BASE_FIELDS;
} ss3UNIT_INFO;

static VOID InfoUnitInit( ss3UNIT *Unit, ss3ANIM *Ani )
{
  wglUseFontBitmaps(Ani->hDC, 0, 256, 111);
}

static VOID InfoUnitClose( ss3UNIT *Unit, ss3ANIM *Ani )
{
}

static VOID InfoUnitResponse( ss3UNIT *Unit, ss3ANIM *Ani )
{
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    SS3_AnimFlipFullScreen();

  if (Ani->KeysClick['P'])
    SS3_AnimSetPause(!Ani->IsPause);

  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);

  if (Ani->Keys[VK_ADD])
  {
    Ani->PosCam.X -= 0.1;
    Ani->PosCam.Y -= 0.1;
    Ani->PosCam.Z -= 0.1;
  }

  if (Ani->Keys[VK_SUBTRACT])
  {
    Ani->PosCam.X += 0.1;
    Ani->PosCam.Y += 0.1;
    Ani->PosCam.Z += 0.1;
  }
}

static VOID InfoUnitRender( ss3UNIT *Unit, ss3ANIM *Ani )
{
  static CHAR Buf[30];

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);

  Ani->MatrWorld = MatrIdentity();
  glLoadMatrixf(Ani->MatrWorld.A[0]);

  glColor3d(1, 1, 1);
  glRasterPos3d(-0.99, 0.95, 0); 
  glListBase(111);

  glCallLists(sprintf(Buf, "FPS: %.3f", Ani->FPS), GL_UNSIGNED_BYTE, Buf);
  glPopAttrib();
}

ss3UNIT * SS3_InfoUnitCreate( VOID )
{
  ss3UNIT_INFO *Unit;

  if ((Unit = (ss3UNIT_INFO *)SS3_AnimUnitCreate(sizeof(ss3UNIT_INFO))) == NULL)
    return NULL;

  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Response = (VOID *)InfoUnitResponse;
  Unit->Render = (VOID *)InfoUnitRender;
  return (ss3UNIT *)Unit;
}