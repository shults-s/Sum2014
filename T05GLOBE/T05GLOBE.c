#include <windows.h>
#include <math.h>

#define WND_CLASS "My Window"

DOUBLE PI = 3.14159265358979323846;

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_INFORMATION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_OK);
    return 0;
  }

  hWnd =
    CreateWindow(WND_CLASS,
                 "Globe",
                 WS_OVERLAPPEDWINDOW,
                 CW_USEDEFAULT , CW_USEDEFAULT,
                 400, 400,
                 NULL,
                 NULL,
                 hInstance,
                 NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}

VOID DrawGlobe(HDC hDC, INT Angle, INT cx, INT cy, INT R)
{
  int x, y, z;
  double fi, tau;

  for(fi = 0; fi < 2 * PI; fi += 0.07)
    for(tau = 0; tau < PI; tau += 0.05)
    {
      x = R * sin(tau) * sin(fi + Angle);
      y = R * cos(tau);
      z = R * sin(tau) * cos(fi + Angle);

      if (z > 0)
        SetPixel(hDC, x + cx, y + cy, RGB(65, 105, 255));
    }
}


VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT n, x, y;
  static INT Angle = 0;
  HDC hDC, hMemDC;
  PAINTSTRUCT ps;
  POINT pt;
  BITMAP bm;
  MINMAXINFO *minmax;
  static HBITMAP hBm;
  static INT W, H;
  BYTE Keys[256];

  GetKeyboardState(Keys);

  if (Keys['S'] & 0x80)
    DestroyWindow(hWnd);

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 7, 70, NULL);
    return 0;

  case WM_CHAR:
    if ((CHAR)wParam == 27)
      DestroyWindow(hWnd);
    if ((CHAR)wParam == 'f')
      FlipFullScreen(hWnd);
    return 0;

  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
    GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;

  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    hDC = GetDC(hWnd);

    if (hBm != NULL)
      DeleteObject(hBm);

    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    return 0;


  case WM_TIMER:
    Angle++;
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;


  case WM_ERASEBKGND:
    return 1;


  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBm);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));
    Rectangle(hMemDC, 0, 0, W, H);

    DrawGlobe(hMemDC, Angle, W / 2, H / 2, H / 3);

    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    DeleteDC(hMemDC);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    DeleteObject(hBm);
    PostQuitMessage(0);
    KillTimer(hWnd, 7);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}