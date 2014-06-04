#include <windows.h>
#include <math.h>

#define WND_CLASS "My Window"

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
                 "Title",
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

VOID DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, FLOAT Angle )
{
  INT i;
  POINT pts[] =
  {
    {0, -W}, {-W, 0}, {0, L}, {W, 0}
  }, pts_draw[sizeof pts / sizeof pts[0]];
  FLOAT si = sin(Angle), co = cos(Angle);

  for (i = 0; i < sizeof pts / sizeof pts[0]; i++)
  {
    pts_draw[i].x = Xc + (pts[i].x * co - pts[i].y * si);
    pts_draw[i].y = Yc - (pts[i].x * si + pts[i].y * co);
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Polygon(hDC, pts_draw, sizeof pts / sizeof pts[0]);
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT n, x, y;
  HDC hDC, hMemDC, hMemDCClock;
  PAINTSTRUCT ps;
  POINT pt;
  BITMAP bm;
  static INT cc = 0;
  static HBITMAP hBmClock, hBmFrame;
  static INT W, H;
  SYSTEMTIME st;
  FLOAT PI = 3.14159265358979323846;



  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 7, 10, NULL);
    hBmClock = LoadImage(NULL, "bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return 0;


  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    hDC = GetDC(hWnd);

    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);

    hBmFrame = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    return 0;


  case WM_TIMER:
    cc++;
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;


  case WM_ERASEBKGND:
    return 1;


  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBmFrame);

    hMemDCClock = CreateCompatibleDC(hDC);
    SelectObject(hMemDCClock, hBmClock);
    GetObject(hBmClock, sizeof(bm), &bm);
    BitBlt(hMemDC, W / 2 - bm.bmWidth / 2, H / 2 - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, hMemDCClock, 0, 0, SRCCOPY);

    GetLocalTime(&st);

    DrawArrow(hMemDC, W / 2, H / 2, 100, 20, (-(st.wHour % 12 + st.wMinute / 60.0) / 12.0) * 2 * PI);
    DrawArrow(hMemDC, W / 2, H / 2, 150, 15, (-(st.wMinute + st.wSecond / 60.0) / 60.0) * 2 * PI);
    DrawArrow(hMemDC, W / 2, H / 2, 150, 3, (-(st.wSecond + st.wMilliseconds / 1000.0) / 60.0) * 2 * PI);


    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    DeleteDC(hMemDC);
    DeleteDC(hMemDCClock);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    DeleteObject(hBmClock);
    DeleteObject(hBmFrame);
    PostQuitMessage(0);
    KillTimer(hWnd, 7);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}