/* FILENAME: ANIM.C
 * PROGRAMMER: RK2
 * PURPOSE: Base animation module
 * LAST UPDATE: 17.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "mth.h"
#include "anim.h"
#include "render.h"
#include "shader.h"

#define RK2_JOYSTICK_AXES_FAULT 10.0
#define RK2_JOYSTICK_AXES_COUNT(Axes) (INT)(RK2_JOYSTICK_AXES_FAULT * ((2.0 * (ji.dw##Axes##pos - jc.w##Axes##min) / (jc.w##Axes##max - jc.w##Axes##min - 1) - 1))) \
        / RK2_JOYSTICK_AXES_FAULT;

/* System animation context */
rk2ANIM RK2_Anim;

/* Synchronisation time data */
static INT64
  RK2_TimeFreq,  /* Large integer time per second */
  RK2_TimeStart, /* Animation time begin */
  RK2_TimeOld,   /* Last frame time */
  RK2_TimePause, /* Pause time */
  RK2_TimeFPS;   /* Time to count FPS */
static INT
  RK2_FrameCounter, /* counter of frames */
  /* Mouse variables */
  RK2_MouseX, RK2_MouseY,             /* Mouse coords */
  RK2_MouseXOld, RK2_MouseYOld,       /* Last mouse coords */
  RK2_MouseWheel;                     /* Mouse wheel state */

/* Animation init function
 * ARGUMENTS:
 *   - Window handle:
 *       HWND hWnd;
 * RETURNS:
 *   (BOOL) Is all correct.
 */
BOOL RK2_AnimInit( HWND hWnd )
{
  LARGE_INTEGER li;
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  RK2_Anim.hWnd = hWnd;
  RK2_Anim.hDC = GetDC(hWnd);
  RK2_Anim.W = 1920;
  RK2_Anim.H = 1080;
  RK2_Anim.NumOfUnits = 0;


  /* Gl init */
  /* Point format declaration */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(RK2_Anim.hDC, &pfd);
  DescribePixelFormat(RK2_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(RK2_Anim.hDC, i, &pfd);

  /* Context build */
  RK2_Anim.hRC = wglCreateContext(RK2_Anim.hDC);
  /* Connect current contexts */
  wglMakeCurrent(RK2_Anim.hDC, RK2_Anim.hRC);

  /* Init extensions */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(RK2_Anim.hRC);
    ReleaseDC(RK2_Anim.hWnd, RK2_Anim.hDC);
    memset(&RK2_Anim, 0, sizeof(rk2ANIM));
    return FALSE;
  }

  /* Timer init */
  QueryPerformanceFrequency(&li);
  RK2_TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  RK2_TimeStart = RK2_TimeOld = RK2_TimeFPS = li.QuadPart;
  RK2_TimePause = 0;
  RK2_FrameCounter = 0;


  /* Parametres of projection */
  
  RK2_Anim.RndWp = 4;
  RK2_Anim.RndHp = 3;                          /* Projection sizes */
  
  RK2_Anim.RndProjDist = 5;                    /* distance to frame */

  RK2_Anim.RndMatrWorld = MatrDefault();       /* Matrix world convertion */
  RK2_Anim.RndMatrView = MatrDefault();        /* Matrix view convertion */
  RK2_Anim.RndMatrProjection = MatrDefault();  /* Projection matrix */
  RK2_Anim.RndMatrRes = MatrDefault();         /* Summed matrix */

  RK2_Anim.ShaderDef = RK2_ShadProgInit("shaders\\default.vert", "shaders\\default.fraq");
  RK2_RndCameraSet(&RK2_Anim.RndCamera, VecSet(60, 60, 60),
                   VecSet(0, 0, 0),
                   VecSet(0, 1, 0));
  return TRUE;
} /* End of 'RK2_AnimInit' function */

/* Animation destructot function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimClose( VOID )
{
  INT i;

  RK2_ShadProgClose(RK2_Anim.ShaderDef);

  /* Units destructor caller */
  for (i = 0; i < RK2_Anim.NumOfUnits; i++)
  {
    RK2_Anim.Units[i]->Close(RK2_Anim.Units[i], &RK2_Anim);
    free(RK2_Anim.Units[i]);
  }

  /* Glut destructor */
  wglMakeCurrent(NULL, NULL);

  wglDeleteContext(RK2_Anim.hRC);

  ReleaseDC(RK2_Anim.hWnd, RK2_Anim.hDC);
  memset(&RK2_Anim, 0, sizeof(rk2ANIM));
} /* End of 'RK2_AnimInit' function */

/* Resize window animation function.
 * ARGUMENTS:
 *   - new size of window:
 *       INT W, H;
 * RETURNS: None.
 */
VOID RK2_AnimResize( INT W, INT H )
{
  DBL ratX = 1, ratY = 1;
  /* Saving new sizes */
  RK2_Anim.W = W;
  RK2_Anim.H = H;

  glViewport(0, 0, W, H);

  if (W > H)
    ratX = (DBL)W / H;
  else
    ratY = (DBL)H / W;

  RK2_Anim.RndWp = ratX;
  RK2_Anim.RndHp = ratY;

  RK2_Anim.RndMatrProjection = MatrProjection(-RK2_Anim.RndWp / 2,
                                              RK2_Anim.RndWp / 2,
                                              -RK2_Anim.RndHp / 2,
                                              RK2_Anim.RndHp / 2,
                                              RK2_Anim.RndProjDist,
                                              1000);
} /* End of 'RK2_AnimResize' function */

/* Mouse main loop messages function.
 * ARGUMENTS:
 *   - Code of catching:
 *       INT Code;
 *   - word parameter - Mouse message code:
 *       WPARAM wParam;
 *   - long parameter - (MSLLHOOKSTRUCT *) for mouse messages:
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) - Depends from message.
 */
static LRESULT CALLBACK RK2_AnimResponseMouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    RK2_MouseX = hs->pt.x;
    RK2_MouseY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    RK2_MouseWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return 0;
} /* End of 'RK2_AnimResponseMouseHook' function */

/* Animation mouse repsonse function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID RK2_AnimResponseMouse( VOID )
{
  POINT pt;
  INT i;

  pt.x= RK2_MouseX;
  pt.y = RK2_MouseY;
  ScreenToClient(RK2_Anim.hWnd, &pt);
  RK2_Anim.MouseX = pt.x;
  RK2_Anim.MouseY = pt.y;
  RK2_Anim.MouseDeltaX = RK2_Anim.MouseX - RK2_Anim.MouseXOld;
  RK2_Anim.MouseDeltaY = RK2_Anim.MouseY - RK2_Anim.MouseYOld;
  RK2_Anim.MouseXOld = RK2_Anim.MouseX;
  RK2_Anim.MouseXOld = RK2_Anim.MouseY;
  RK2_Anim.MouseWheel = RK2_MouseWheel;

  /* Update input system */

  memcpy(RK2_Anim.KeysOld, RK2_Anim.Keys, sizeof(RK2_Anim.Keys));

  GetKeyboardState(RK2_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    RK2_Anim.Keys[i] >>= 7;
    if (RK2_Anim.KeysOld[i] && RK2_Anim.Keys[i])
      RK2_Anim.KeysClicked[i] = TRUE;
    else
      RK2_Anim.KeysClicked[i] = FALSE;
  }
} /* End of 'RK2_AnimResponseMouse' function */

/* Animation joystick repsonse function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID RK2_AnimResponseJoystick( VOID )
{
  INT i;
  static INT Fault = 10;
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* Getting joysticks information */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* Getting joystick #1 current state */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        memcpy(RK2_Anim.JoyButsOld, RK2_Anim.JoyButs, sizeof(RK2_Anim.JoyButs));
        for (i = 0; i < 32; i++)
          RK2_Anim.JoyButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          RK2_Anim.JoyButsClicked[i] = RK2_Anim.JoyButs[i] && !RK2_Anim.JoyButsOld[i];

        /* Axis */
        RK2_Anim.JoyX = RK2_JOYSTICK_AXES_COUNT(X);
        RK2_Anim.JoyY = RK2_JOYSTICK_AXES_COUNT(Y);
        
        if (jc.wCaps & JOYCAPS_HASZ)
          RK2_Anim.JoyZ = RK2_JOYSTICK_AXES_COUNT(Z);
        if (jc.wCaps & JOYCAPS_HASR)
          RK2_Anim.JoyR = RK2_JOYSTICK_AXES_COUNT(R);
        if (jc.wCaps & JOYCAPS_HASU)
          RK2_Anim.JoyU = RK2_JOYSTICK_AXES_COUNT(U);

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            RK2_Anim.JoyPOV = 0;
          else
            RK2_Anim.JoyPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }
} /* End of 'RK2_AnimResponseJoystick' function */

/* Animation rendering function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  /* Update timer */
  RK2_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  QueryPerformanceCounter(&li);

  /* global time */
  RK2_Anim.GlobalTime = (DBL)(li.QuadPart - RK2_TimeStart) / RK2_TimeFreq;
  RK2_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - RK2_TimeOld) / RK2_TimeFreq;

  /* local time */
  if (RK2_Anim.IsPause)
  {
    RK2_TimePause += li.QuadPart - RK2_TimeOld;
    RK2_Anim.DeltaTime = 0;
  }
  else
    RK2_Anim.DeltaTime = RK2_Anim.GlobalDeltaTime;

  RK2_Anim.Time = (DBL)(li.QuadPart - RK2_TimeStart - RK2_TimePause) / RK2_TimeFreq;

  /* counting FPS */
  if (li.QuadPart - RK2_TimeFPS > RK2_TimeFreq)
  {
    RK2_Anim.FPS = RK2_FrameCounter / ((DBL)(li.QuadPart - RK2_TimeFPS) / RK2_TimeFreq);
    RK2_TimeFPS = li.QuadPart;
    RK2_FrameCounter = 0;
  }

  /* setting last frame time */
  RK2_TimeOld = li.QuadPart;

  /* Mouse response */
  RK2_AnimResponseMouse();
  /* Joystick response */
  RK2_AnimResponseJoystick();

  glDepthMask(TRUE);
  glClearDepth(1);
  glDepthRange(-100, 1000);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT);

  /* Clearing background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.3, 0.5, 0.7, 1);

  /* Response caller for units */
  for (i = 0; i < RK2_Anim.NumOfUnits; i++)
    RK2_Anim.Units[i]->Response(RK2_Anim.Units[i], &RK2_Anim);

  /* Drawing objects */
  glDisable(GL_CULL_FACE);
  glEnable(GL_ALPHA);
  /// AlphaFunc(GL_SRC_ALPHA, GL_ONE);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /// glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
  /// glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  for (i = 0; i < RK2_Anim.NumOfUnits; i++)
    RK2_Anim.Units[i]->Render(RK2_Anim.Units[i], &RK2_Anim);

  RK2_FrameCounter++;
} /* End of 'RK2_AnimRender' function */

/* Drawing to frame animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimCopyFrame( VOID )
{
  /* Drawing frame */
  SwapBuffers(RK2_Anim.hDC);
} /* End of 'RK2_AnimCopyFrame' function */

/* Adding object to animation list function.
 * ARGUMENTS:
 *   - new object:
 *       rk2UNIT *Unit;
 * RETURNS: None.
 */
VOID RK2_AnimAddUnit( rk2UNIT *Unit )
{
  if (RK2_Anim.NumOfUnits < RK2_MAX_UNITS)
  {
    RK2_Anim.Units[RK2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &RK2_Anim);
  }
} /* End of 'RK2_AnimAddUnit' function */

/* Switching to/from fullscreen function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* current regime */
  static RECT SaveRC;               /* saved size */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    /* MONITORINFOEX moninfo;  For WinXP */
    MONITORINFO moninfo;      /* Windows 8 */

    /* Old sizes saving */
    GetWindowRect(RK2_Anim.hWnd, &SaveRC);

    /* choosing monitor */
    hmon = MonitorFromWindow(RK2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Getting information about current monitor */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Switch to fullscreen */
    /* for one monitor:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;
    
    AdjustWindowRect(&rc, GetWindowLong(RK2_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(RK2_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* Update last sizes */
    SetWindowPos(RK2_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'RK2_AnimFlipFullScreen' function */

/* Setting animation pause function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_AnimSetPause( VOID )
{
  static Pause = FALSE;
  Pause = !Pause;
  RK2_Anim.IsPause = Pause;
} /* End of 'RK2_AnimSetPause' function */

/* Render setting function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RK2_RndBuildMatrix( VOID )
{
  RK2_Anim.RndMatrView = MatrViewLookAt(RK2_Anim.RndCamera.Loc, RK2_Anim.RndCamera.At, RK2_Anim.RndCamera.Up);
  RK2_Anim.RndMatrRes = MatrMultMatr(MatrMultMatr(RK2_Anim.RndMatrWorld, RK2_Anim.RndMatrView), RK2_Anim.RndMatrProjection);
} /* End of 'RK2_RndBuildMatrixView' function */

/* END OF 'ANIM.C' FILE */
