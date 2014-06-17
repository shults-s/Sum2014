/* FILENAME: ANIM.C
 * PROGRAMMER: SS3
 * PURPOSE: Base animation module
 * LAST UPDATE: 09.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"
#include <mmsystem.h>

#define SS3_JOYSTICK_THRESHOLD 128.0

#define SS3_GET_AXIS_VALUE(A) \
  ((INT)(SS3_JOYSTICK_THRESHOLD * (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ##min - 1) - 1) + 0.5) / SS3_JOYSTICK_THRESHOLD)

static ss3ANIM SS3_Anim;

static INT64
  TimeFreq,
  TimeStart,
  TimeOld,
  TimePause,
  TimeFPS;
static INT
  FrameCounter;

static INT
  SS3_MouseGlobalX, SS3_MouseGlobalY,
  SS3_MouseXOld, SS3_MouseYOld,
  SS3_MouseGlobalWheel;

static HHOOK SS3_hMouseHook;

static LRESULT CALLBACK SS3_MouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    SS3_MouseGlobalX = hs->pt.x;
    SS3_MouseGlobalY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    SS3_MouseGlobalWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return 0;
}

VOID SS3_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  SS3_Anim.hWnd = hWnd;
  SS3_Anim.hDC = CreateCompatibleDC(hDC);
  SS3_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SS3_Anim.W = 30;
  SS3_Anim.H = 30;
  SS3_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;

  //SS3_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, SS3_MouseHook, GetModuleHandle(NULL), 0);
}

VOID SS3_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < SS3_Anim.NumOfUnits; i++)
  {
    SS3_Anim.Units[i]->Close(SS3_Anim.Units[i], &SS3_Anim);
    free(SS3_Anim.Units[i]);
  }
  DeleteObject(SS3_Anim.hBmFrame);
  DeleteDC(SS3_Anim.hDC);
  memset(&SS3_Anim, 0, sizeof(ss3ANIM));

  //UnhookWindowsHookEx(SS3_hMouseHook);
}

VOID SS3_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(SS3_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(SS3_Anim.hBmFrame);
  SS3_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(SS3_Anim.hDC, SS3_Anim.hBmFrame);

  /* Сохранение размера */
  SS3_Anim.W = W;
  SS3_Anim.H = H;

  ReleaseDC(SS3_Anim.hWnd, hDC);
}

VOID SS3_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  GetKeyboardState(SS3_Anim.Keys);
  for (i = 0; i < 256; i++)
    SS3_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    SS3_Anim.KeysClick[i] = SS3_Anim.Keys[i] && !SS3_Anim.KeysOld[i];
  memcpy(SS3_Anim.KeysOld, SS3_Anim.Keys, 256);

  SS3_Anim.MsWheel = SS3_MouseGlobalWheel;
  SS3_MouseGlobalWheel = 0;

  pt.x = SS3_MouseGlobalX;
  pt.y = SS3_MouseGlobalY;
  ScreenToClient(SS3_Anim.hWnd, &pt);
  SS3_Anim.MsX = pt.x;
  SS3_Anim.MsY = pt.y;

  SS3_Anim.MsDeltaX = SS3_MouseGlobalX - SS3_MouseXOld;
  SS3_Anim.MsDeltaY = SS3_MouseGlobalY - SS3_MouseYOld;
  SS3_MouseXOld = SS3_MouseGlobalX;
  SS3_MouseYOld = SS3_MouseGlobalY;

  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(SS3_Anim.JButsOld, SS3_Anim.JButs, sizeof(SS3_Anim.JButs));
        for (i = 0; i < 32; i++)
          SS3_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          SS3_Anim.JButsClick[i] = SS3_Anim.JButs[i] && !SS3_Anim.JButsOld[i];

        /* Оси */
        SS3_Anim.JX = SS3_GET_AXIS_VALUE(X);
        SS3_Anim.JY = SS3_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          SS3_Anim.JZ = SS3_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASR)
          SS3_Anim.JR = SS3_GET_AXIS_VALUE(R);
        if (jc.wCaps & JOYCAPS_HASU)
          SS3_Anim.JU = 2.0 * (ji.dwUpos - jc.wUmin) / (jc.wUmax - jc.wUmin - 1) - 1;

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            SS3_Anim.JPOV = 0;
          else
            SS3_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  SS3_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  QueryPerformanceCounter(&li);

  SS3_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  SS3_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (SS3_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    SS3_Anim.DeltaTime = 0;
  }
  else
    SS3_Anim.DeltaTime = SS3_Anim.GlobalDeltaTime;

  SS3_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    SS3_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /* очистка фона */
  SelectObject(SS3_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(SS3_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(SS3_Anim.hDC, RGB(0, 0, 0));
  Rectangle(SS3_Anim.hDC, 0, 0, SS3_Anim.W, SS3_Anim.H);

  /* опрос на изменение состояний объектов */
  for (i = 0; i < SS3_Anim.NumOfUnits; i++)
    SS3_Anim.Units[i]->Response(SS3_Anim.Units[i], &SS3_Anim);

  /* рисование объектов */
  for (i = 0; i < SS3_Anim.NumOfUnits; i++)
  {
    SelectObject(SS3_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(SS3_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(SS3_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(SS3_Anim.hDC, RGB(0, 0, 0));
    SS3_Anim.Units[i]->Render(SS3_Anim.Units[i], &SS3_Anim);
  }

  FrameCounter++;
}

VOID SS3_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(SS3_Anim.hWnd);
  BitBlt(hDC, 0, 0, SS3_Anim.W, SS3_Anim.H, SS3_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(SS3_Anim.hWnd, hDC);
}

VOID SS3_AnimAddUnit( ss3UNIT *Unit )
{
  if (SS3_Anim.NumOfUnits < SS3_MAX_UNITS)
  {
    SS3_Anim.Units[SS3_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &SS3_Anim);
  }
}

VOID SS3_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    // сохраняем старый размер окна
    GetWindowRect(SS3_Anim.hWnd, &SaveRC);

    // определяем в каком мониторе находится окно
    hmon = MonitorFromWindow(SS3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    // получаем информацию для монитора
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(SS3_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(SS3_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    // восстанавливаем размер окна
    SetWindowPos(SS3_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
}

VOID SS3_AnimSetPause( BOOL NewPauseFlag )
{
  SS3_Anim.IsPause = NewPauseFlag;
}