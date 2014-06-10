/* FILENAME: STARTUP.C
 * PROGRAMMER: SS3
 * PURPOSE: Animation startup module
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

#define WND_CLASS "My Window"

LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  INT i;

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_INFORMATION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS, "Animation",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT , CW_USEDEFAULT,
    CW_USEDEFAULT , CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL);

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  //SS3_AnimAddUnit(SS3_ClocksUnitCreate());
  SS3_AnimAddUnit(SS3_InfoUnitCreate());
  SS3_AnimAddUnit(SS3_MyUnitCreate());
  SS3_AnimAddUnit(SS3_CubeUnitCreate());

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}

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
    SS3_AnimInit(hWnd);
    return 0;

  case WM_CHAR:
    switch ((CHAR)wParam)
    {
    case 27:
      DestroyWindow(hWnd);
      return 0;
    case 'f':
      SS3_AnimFlipFullScreen(hWnd);
      return 0;
    case 'p':
      SS3_AnimSetPause(pause = !pause);
      return 0;
    }
    return 0;

  case WM_SIZE:
    SS3_AnimResize(LOWORD(lParam), HIWORD(lParam));
    return 0;

  case WM_TIMER:
    SS3_AnimRender();
    SS3_AnimCopyFrame();
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    SS3_AnimCopyFrame();
    return 0;

  case WM_DESTROY:
    SS3_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}