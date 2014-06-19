/* FILENAME: MAIN.C
 * PROGRAMMER: EF2
 * PURPOSE: Animation startup module
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"
#include "render.h"

#define WND_CLASS_NAME "Super Animation System"

/* Declaration of window function */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

/* Main function of programme.
 * ARGUMENTS:
 *   - hInstance of programme:
 *       HINSTANCE hInstance;
 *   - preview hInstance of programme:
 *       HINSTANCE hPrevInstance;
 *   - command line:
 *       CHAR *CmdLine;
 *   - flag to show screen:
 *       INT ShowCmd;
 * RETURNS:
 *   (INT) returned number for opertion sistem.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* Registar of class */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0; /* baits for class */
  wc.cbWndExtra = 0; /* baits for hWnd */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* Register class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* Create hWnd */
  hWnd = CreateWindow(WND_CLASS_NAME, "First Window Sample",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 
    CW_USEDEFAULT, CW_USEDEFAULT, 
    NULL,                         
    NULL,                         
    hInstance,                    
    NULL);                        

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /*** Added unit ***/
  //for (i = 0; i < 30 * 30; i++)
  //  EF2_AnimAddUnit(EF2_CowUnitCreate());
  EF2_AnimAddUnit(EF2_GobjUnitCreate());
  EF2_AnimAddUnit(EF2_InfoUnitCreate());
  EF2_AnimAddUnit(EF2_PlansUnitCreate());
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      /* Message for programm */
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 0, 0);

  return msg.wParam;
} /* End of 'WinMain' function */

/* Функция обработки сообщения окна.
 * ARGUMENTS:
 *   - hWnd:
 *       HWND hWnd;
 *   - Number of message:
 *       UINT Msg;
 *   - word parameter:
 *       WPARAM wParam;
 *   - long parameter:
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) - return message.
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
    if (!EF2_AnimInit(hWnd))
      exit(0);
    return 0;
  case WM_SIZE:
    EF2_AnimResize(LOWORD(lParam), HIWORD(lParam));
  case WM_TIMER:
    EF2_AnimRender();
    EF2_AnimCopyFrame();
    return 0;
  case WM_CHAR:
    switch ((CHAR)wParam)
    {
    case 27:
      DestroyWindow(hWnd);
      return 0;
    case 'f':
      EF2_AnimFlipFullScreen();
      return 0;
    case 'p':
      EF2_AnimSetPause(pause = !pause);
      return 0;
    }
    return 0;
  case WM_ERASEBKGND:
    EF2_AnimCopyFrame();
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    EF2_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'MAIN.C' FILE */
