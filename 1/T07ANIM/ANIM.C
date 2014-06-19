/* FILENAME: ANIM.C
 * PROGRAMMER: EF2
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"

/* Точность вычисления позиции джойстика */
#define EF2_JOYSTICK_THRESHOLD 8.0

/* Устранение ошибок замера джойстика */
#define EF2_GET_AXIS_VALUE(A) \
  ((INT)(EF2_JOYSTICK_THRESHOLD * (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ##min - 1) - 1) + 0.5) / EF2_JOYSTICK_THRESHOLD)

/* System contecs of animation */
ef2ANIM EF2_Anim;

/* Data for sinhronize time */
static INT64
  TimeFreq,     /* CLOCKS_PER_SEC */
  TimeStart,    /* begin of animation */
  TimeOld,      /* time of old frame */
  TimePause,    /* time of pause */
  TimeFPS;      /* time for FPS */
static INT
  FrameCounter; /* counter of frames */

/* Data for mouse */
static INT
  EF2_MouseGlobalX, EF2_MouseGlobalY, /* coordinates */
  EF2_MouseXOld, EF2_MouseYOld,       /* From last message */
  EF2_MouseGlobalWheel;               /* Situation for mouse wheel */

/* deskriptor for mouse message */
static HHOOK EF2_hMouseHook;

UINT EF2_ShaderProg;

/* Catch message of mouse function
 * ARGUMENTS:
 *   - code of catch:
 *       INT Code;
 *   - parametr of message ('word parameter') -
 *     cod message of mouse:
 *       WPARAM wParam;
 *   - parametr of message ('long parameter') -
 *     (MSLLHOOKSTRUCT *) for message of mouse;
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) - result of message.
 */
static LRESULT CALLBACK EF2_MouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    EF2_MouseGlobalX = hs->pt.x;
    EF2_MouseGlobalY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    EF2_MouseGlobalWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return 0;
} /* End of 'EF2_MouseHook' function */

/* Init animation function.
 * ARGUMENTS:
 *   - hWnd of window:
 *       HWND hWnd;
 * RETURNS: None.
 */
BOOL EF2_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER li;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /* init of screen parametrs */
  EF2_Anim.hDC = GetDC(hWnd);
  EF2_Anim.hWnd = hWnd;
  EF2_Anim.W = 30;
  EF2_Anim.H = 30;
  EF2_Anim.NumOfUnits = 0;
  
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(EF2_Anim.hDC, &pfd);
  DescribePixelFormat(EF2_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(EF2_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  EF2_Anim.hRC = wglCreateContext(EF2_Anim.hDC);
  /* делаем текущими контексты */
  wglMakeCurrent(EF2_Anim.hDC, EF2_Anim.hRC);

  /* init glew.h */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(EF2_Anim.hRC);
    ReleaseDC(EF2_Anim.hWnd, EF2_Anim.hDC);
    memset(&EF2_Anim, 0, sizeof(ef2ANIM));
    return FALSE;
  }

  /* default parametrs for OpenGL */

  /* init of timer */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;

  /* Параметры проецирования */
  EF2_Anim.Wp = 4, EF2_Anim.Hp = 3,     /* размеры обрасти проецирования */
  EF2_Anim.ProjDist = 5;              /* расстояние до плоскости проекции */
  EF2_Anim.MatrWorld =  
    EF2_Anim.MatrView = 
    EF2_Anim.MatrProjection = MatrIdenity();
  EF2_Anim.ProjSize = 1;
  EF2_Anim.PosCam.X = EF2_Anim.PosCam.Y = EF2_Anim.PosCam.Z = 25;
  /* Init mouse hook */
  EF2_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, EF2_MouseHook, GetModuleHandle(NULL), 0);
  return TRUE;
} /* End of 'EF2_AnimInit' function */

/* Uninit animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EF2_AnimClose( VOID )
{
  INT i;

  /* Free memory from unit of animation */
  for (i = 0; i < EF2_Anim.NumOfUnits; i++)
  {
    EF2_Anim.Units[i]->Close(EF2_Anim.Units[i], &EF2_Anim);
    free(EF2_Anim.Units[i]);
  }
  /* offline context */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(EF2_Anim.hRC);
  ReleaseDC(EF2_Anim.hWnd, EF2_Anim.hDC);
  memset(&EF2_Anim, 0, sizeof(ef2ANIM));

  /* Отключение ловушки для сообщений для мыши */
  UnhookWindowsHookEx(EF2_hMouseHook);
} /* End of 'EF2_AnimInit' function */

/* Resize of animation function.
 * ARGUMENTS:
 *   - new width and height of animation:
 *       INT W, H;
 * RETURNS: None.
 */
VOID EF2_AnimResize( INT W, INT H )
{
  DBL ratio_x = 1, ratio_y = 1;
  /* Save size of screen */
  EF2_Anim.W = W;
  EF2_Anim.H = H;

  if (W > H)
    ratio_x = (DBL)W / H;
  else
    ratio_y = (DBL)H / W;
  EF2_Anim.Wp = EF2_Anim.ProjSize * ratio_x;
  EF2_Anim.Hp = EF2_Anim.ProjSize * ratio_y;
  EF2_Anim.MatrProjection = MatrProjection(-EF2_Anim.Wp / 2, EF2_Anim.Wp / 2, -EF2_Anim.Hp / 2, EF2_Anim.Hp / 2, EF2_Anim.ProjDist, 1000);
  /* Change size of screen */
  glViewport(0, 0, W, H);
} /* End of 'EF2_AnimResize' function */

/* Render farme of animation.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EF2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  /* Update keys */
  GetKeyboardState(EF2_Anim.Keys);
  for (i = 0; i < 256; i++)
    EF2_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    EF2_Anim.KeysClick[i] = EF2_Anim.Keys[i] && !EF2_Anim.KeysOld[i];
  memcpy(EF2_Anim.KeysOld, EF2_Anim.Keys, 256);

  /* Мышь */
  /*  колесо */
  EF2_Anim.MsWheel = EF2_MouseGlobalWheel;
  EF2_MouseGlobalWheel = 0;
  /* абсолютная позиция */
  pt.x = EF2_MouseGlobalX;
  pt.y = EF2_MouseGlobalY;
  ScreenToClient(EF2_Anim.hWnd, &pt);
  EF2_Anim.MsX = pt.x;
  EF2_Anim.MsY = pt.y;
  /* относительное перемещение */
  EF2_Anim.MsDeltaX = EF2_MouseGlobalX - EF2_MouseXOld;
  EF2_Anim.MsDeltaY = EF2_MouseGlobalY - EF2_MouseYOld;
  EF2_MouseXOld = EF2_MouseGlobalX;
  EF2_MouseYOld = EF2_MouseGlobalY;

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
        memcpy(EF2_Anim.JButsOld, EF2_Anim.JButs, sizeof(EF2_Anim.JButs));
        for (i = 0; i < 32; i++)
          EF2_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          EF2_Anim.JButsClick[i] = EF2_Anim.JButs[i] && !EF2_Anim.JButsOld[i];

        /* Оси */
        EF2_Anim.JX = EF2_GET_AXIS_VALUE(X);
        EF2_Anim.JY = EF2_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          EF2_Anim.JZ = EF2_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASR)
          EF2_Anim.JR = EF2_GET_AXIS_VALUE(R);
        if (jc.wCaps & JOYCAPS_HASU)
          EF2_Anim.JU = 2.0 * (ji.dwUpos - jc.wUmin) / (jc.wUmax - jc.wUmin - 1) - 1;

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            EF2_Anim.JPOV = 0;
          else
            EF2_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  /* Update of time */
  EF2_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* Update frame */
  QueryPerformanceCounter(&li);

  /* Global time */
  EF2_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  EF2_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* Local time */
  if (EF2_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    EF2_Anim.DeltaTime = 0;
  }
  else
    EF2_Anim.DeltaTime = EF2_Anim.GlobalDeltaTime;

  EF2_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* count FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    EF2_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* time of old frame */
  TimeOld = li.QuadPart;
  /* Answered responsible peremenn */
  for (i = 0; i < EF2_Anim.NumOfUnits; i++)
    EF2_Anim.Units[i]->Response(EF2_Anim.Units[i], &EF2_Anim);

  /* clean brush */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(GL_DEPTH_BUFFER_BIT);
  /* draw of units */
  for (i = 0; i < EF2_Anim.NumOfUnits; i++)
    EF2_Anim.Units[i]->Render(EF2_Anim.Units[i], &EF2_Anim);

  FrameCounter++;
} /* End of 'EF2_AnimRender' function */

/* Output frame of animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EF2_AnimCopyFrame( VOID )
{
  glFinish();
  SwapBuffers(EF2_Anim.hDC);
} /* End of 'EF2_AnimCopyFrame' function */

/* Add unit to animation function.
 * ARGUMENTS:
 *   - added unit of animation:
 *       ef2UNIT *Unit;
 * RETURNS: None.
 */
VOID EF2_AnimAddUnit( ef2UNIT *Unit )
{
  if (EF2_Anim.NumOfUnits < EF2_MAX_UNITS)
  {
    EF2_Anim.Units[EF2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &EF2_Anim);
  }
} /* End of 'EF2_AnimAddUnit' function */

/* Do fullscreen in manyscreens function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID EF2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; 
  static RECT SaveRC;               

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* save old orientation of screen */
    GetWindowRect(EF2_Anim.hWnd, &SaveRC);

    hmon = MonitorFromWindow(EF2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* ger information of screen */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(EF2_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(EF2_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* update size of screen */
    SetWindowPos(EF2_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'EF2_AnimFlipFullScreen' function */

/* Set pause in animation function.
 * ARGUMENTS:
 *   - flag of pause:
 *       BOOL NewPauseFlag;
 * RETURNS: None.
 */
VOID EF2_AnimSetPause( BOOL NewPauseFlag )
{
  EF2_Anim.IsPause = NewPauseFlag;
} /* End of 'EF2_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */
