/* FILENAME: MAIN.C
 * PROGRAMMER: RK2
 * PURPOSE: Animation startup module
 * LAST UPDATE: 17.06.2014
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "def.h"

#include "mth.h"
#include "anim.h"
#include "units.h"

#define WND_CLASS_NAME "My Window Class Name"

/* Declaration Main Window Loop function */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

/* Main program function.
 * ARGUMENTS:
 *   - Descriptor of application:
 *       HINSTANCE hInstance;
 *   - Descriptor of last application:
 *     (Don't use in WinNT: NULL):
 *       HINSTANCE hPrevInstance;
 *   - Command line:
 *       CHAR *CmdLine;
 *   - Flag of showing window (см. SW_SHOWNORMAL, SW_SHOWMINIMIZED, SW_***):
 *       INT ShowCmd;
 * RETURNS:
 *   (INT) Return code to system.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  INT i;

  /* Main window class registration */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0; /* Extra count of bytes for class */
  wc.cbWndExtra = 0; /* Extra count of bytes for windows */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* Background */
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* Class register */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* Creating window */
  hWnd = CreateWindow(WND_CLASS_NAME, "Main Window",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, /* window position - default */
    CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - default */
    NULL,                         /* Parent window descriptor */
    NULL,                         /* Menu descriptor */
    hInstance,                    /* Application Descriptor */
    NULL);                        /* Another parametres */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /*** Adding units ***/

  RK2_AnimAddUnit(RK2_UnitAnimContolCreate());
  RK2_AnimAddUnit(RK2_UnitAnimInfoCreate());
  RK2_AnimAddUnit(RK2_UnitCamFlyCreate());
  RK2_AnimAddUnit(RK2_UnitCorSysCreate());
  //RK2_AnimAddUnit(RK2_UnitGrassCreate());
  /* RK2_AnimAddUnit(RK2_UnitAreaCreate("..\\Textures\\water.bmp", 
                                     VecSet(0, 0, 0), VecSet(0, 0, 0), VecSet(0, 0, 0), VecSet(0, 0, 0)));
  */

  /// RK2_AnimAddUnit(RK2_UnitHeightMapCreate("../Textures/heightmap0.bmp", "../Textures/grass.bmp"));
  RK2_AnimAddUnit(RK2_UnitHeightMapCreate("../Textures/heightmap0.bmp", "../Textures/grass3.bmp"));
  /// RK2_AnimAddUnit(RK2_UnitHeightMapCreate("../Textures/heightmap0.bmp", "../Textures/heightmap0.bmp"));

  RK2_AnimAddUnit(RK2_UnitBoatCreate(50, 100, 170, 0, 0, 0));

  // RK2_AnimAddUnit(RK2_UnitGObjCreate("..\\gobjects\\boat0\\obj\\boat.object",        /* Obj */
     //                                 0, 40, 0,                                       /* Pos */
       //                               0, 0, 0,                                       /* Rotation */
         //                             NULL, NULL));                                  /* Shaders */

  
  // RK2_AnimAddUnit(RK2_UnitGObjCreate("..\\gobjects\\seagul.object",                    /* Obj */
  //                                   0, 100, 0,                                       /* Pos */
    //                                 0, 0, 0,                                       /* Rotation */
      //                               NULL, NULL));  /* Shaders */
  
  // RK2_AnimAddUnit(RK2_UnitGObjCreate("..\\gobjects\\houses\\house1.object",                    /* Obj */
    //                                 10, 100, 0,                                       /* Pos */
      //                               0, 0, 0,                                       /* Rotation */
        //                             NULL, NULL));  /* Shaders */

  /// RK2_AnimAddUnit(RK2_UnitGObjCreate("..\\gobjects\\house1.object", 0, 0, 40, 0, 0, 0, NULL, NULL));
  /// RK2_AnimAddUnit(RK2_UnitGObjCreate("gobjects\\porsche.object", 40, 0, 0, 0, 0, 0));

  /*
  for (i = 0; i < 10; i++)
    RK2_AnimAddUnit(RK2_UnitGObjCreate("..\\gobjects\\seagul.object", 
                                       rand() % 20 - 10, rand() % 20 + 10, rand() % 20 - 10, 
                                       rand() % 360, rand() % 360, rand() % 360, 
                                       NULL, NULL));
  */
  //RK2_AnimAddUnit(RK2_UnitGObjCreate("gobjects\\seagul.object", rand() % 20 - 10, rand() % 20 + 10, rand() % 20 - 10, 0, 0, 0, NULL, NULL));

  /* Last */
  RK2_AnimAddUnit(RK2_UnitAreaCreate("..\\Textures\\water.bmp", 
                                     VecSet(0, 0, 0), VecSet(0, 0, 0), VecSet(0, 0, 0), VecSet(0, 0, 0)));

  /* Main message loop running */
  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      /* Sending messages */
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 0, 0);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* Main window message loop function.
 * ARGUMENTS:
 *   - Window descriptor:
 *       HWND hWnd;
 *   - Message ID:
 *       UINT Msg;
 *   - word message parameter:
 *       WPARAM wParam;
 *   - long message parameter::
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) - depends from message.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;
  static BOOL pause;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    RK2_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    RK2_AnimResize(LOWORD(lParam), HIWORD(lParam));
  case WM_TIMER:
    RK2_AnimRender();
    RK2_AnimCopyFrame();
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    RK2_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    RK2_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'MAIN.C' FILE */
