/* SS3 02.06.14 */

#include <math.h>
#include <windows.h>

#define SQR(X) ((X) * (X))
#define WND_CLASS_NAME "My Window"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }


  hWnd = CreateWindow(WND_CLASS_NAME, "Eyes",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL);

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);


  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}

float r0( void )
{
  return (float)rand() / RAND_MAX;
}

void DrawEye( HDC hDC, INT Xm, INT Ym, INT Xc, INT Yc, INT W, INT H, INT R )
{
  INT xx, yy;
  float
    len, si, co,
    ratio = (float)W / H;

  len = sqrt(SQR(Xc - Xm) + SQR((Yc - Ym) * ratio));

  if (len == 0)
    si = 0, co = 0;
  else
    co = (Xm - Xc) / len, si = (Ym - Yc) * ratio / len;

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  Ellipse(hDC, Xc - W / 2, Yc - H / 2, Xc + W / 2, Yc + H / 2);

  if (len > W / 2 - R)
    len = W / 2 - R;
  xx = Xc + co * len;
  yy = Yc + si * len / ratio;

  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, xx - R, yy - R / ratio, xx + R, yy + R / ratio);
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT n, x, y;
  HDC hDC, hMemDC;
  PAINTSTRUCT ps;
  POINT pt;
  BITMAP bm;
  static HBITMAP hBm;
  static INT W, H;

  switch (Msg)
  {
  case WM_CHAR:
    if ((CHAR)wParam == 27)
      DestroyWindow(hWnd);
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 50, NULL);
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
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBm);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    Rectangle(hMemDC, 0, 0, W, H);

    DrawEye(hMemDC, pt.x, pt.y, W / 4, H / 4 + 100, W / 2 - 70, H / 2 + 100, H / 16);
    DrawEye(hMemDC, pt.x, pt.y, W / 1.4, H / 4 + 100, W / 2 - 70, H / 2 + 100, H / 16);

    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    DeleteDC(hMemDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}