/* FILENAME: ANIM.C
 * PROGRAMMER: SS3
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"

/* Точность вычисления позиции джойстика */
#define SS3_JOYSTICK_THRESHOLD 256.0

/* Устранение ошибок замера джойстика */
#define SS3_GET_AXIS_VALUE(A) \
  ((INT)(SS3_JOYSTICK_THRESHOLD * (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ##min - 1) - 1) + 0.5) / SS3_JOYSTICK_THRESHOLD)

ss3ANIM SS3_Anim;

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

UINT SS3_ShaderProg;

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

BOOL SS3_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER li;
  PIXELFORMATDESCRIPTOR pfd = {0};

  SS3_Anim.hDC = GetDC(hWnd);
  SS3_Anim.hWnd = hWnd;
  SS3_Anim.W = 30;
  SS3_Anim.H = 30;
  SS3_Anim.NumOfUnits = 0;
  
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(SS3_Anim.hDC, &pfd);
  DescribePixelFormat(SS3_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(SS3_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  SS3_Anim.hRC = wglCreateContext(SS3_Anim.hDC);

  /* делаем текущими контексты */
  wglMakeCurrent(SS3_Anim.hDC, SS3_Anim.hRC);

  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(SS3_Anim.hRC);
    ReleaseDC(SS3_Anim.hWnd, SS3_Anim.hDC);
    memset(&SS3_Anim, 0, sizeof(ss3ANIM));
    return FALSE;
  }

  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;

  /* Параметры проецирования */
  SS3_Anim.Wp = 4, SS3_Anim.Hp = 3,     /* размеры обрасти проецирования */
  SS3_Anim.ProjDist = 5;              /* расстояние до плоскости проекции */
  SS3_Anim.MatrWorld =  
    SS3_Anim.MatrView = 
    SS3_Anim.MatrProjection = MatrIdentity();
  SS3_Anim.ProjSize = 1;
  SS3_Anim.PosCam.X = SS3_Anim.PosCam.Y = SS3_Anim.PosCam.Z = 50;

  //SS3_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, SS3_MouseHook, GetModuleHandle(NULL), 0);
  return TRUE;
}

VOID SS3_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < SS3_Anim.NumOfUnits; i++)
  {
    SS3_Anim.Units[i]->Close(SS3_Anim.Units[i], &SS3_Anim);
    free(SS3_Anim.Units[i]);
  }

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(SS3_Anim.hRC);
  ReleaseDC(SS3_Anim.hWnd, SS3_Anim.hDC);
  memset(&SS3_Anim, 0, sizeof(ss3ANIM));

  /* Отключение ловушки для сообщений для мыши */
  //UnhookWindowsHookEx(SS3_hMouseHook);
}

VOID SS3_AnimResize( INT W, INT H )
{
  DBL ratio_x = 1, ratio_y = 1;

  SS3_Anim.W = W;
  SS3_Anim.H = H;

  if (W > H)
    ratio_x = (DBL)W / H;
  else
    ratio_y = (DBL)H / W;
  SS3_Anim.Wp = SS3_Anim.ProjSize * ratio_x;
  SS3_Anim.Hp = SS3_Anim.ProjSize * ratio_y;
  SS3_Anim.MatrProjection = MatrProjection(-SS3_Anim.Wp / 2, SS3_Anim.Wp / 2, -SS3_Anim.Hp / 2, SS3_Anim.Hp / 2, SS3_Anim.ProjDist, 1000);
  glViewport(0, 0, W, H);
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

  /* Мышь */
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

  /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
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

  TimeOld = li.QuadPart;
  for (i = 0; i < SS3_Anim.NumOfUnits; i++)
    SS3_Anim.Units[i]->Response(SS3_Anim.Units[i], &SS3_Anim);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0, 0, 0, 1);
  glClear(GL_DEPTH_BUFFER_BIT);
  for (i = 0; i < SS3_Anim.NumOfUnits; i++)
    SS3_Anim.Units[i]->Render(SS3_Anim.Units[i], &SS3_Anim);

  FrameCounter++;
}

VOID SS3_AnimCopyFrame( VOID )
{
  glFinish();
  SwapBuffers(SS3_Anim.hDC);
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

    GetWindowRect(SS3_Anim.hWnd, &SaveRC);

    hmon = MonitorFromWindow(SS3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

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