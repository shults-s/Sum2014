#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <math.h>

#define WND_CLASS "My Window"

LRESULT CALLBACK MyWindowFunc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd)
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
                 CW_USEDEFAULT, CW_USEDEFAULT,
                 CW_USEDEFAULT, CW_USEDEFAULT,
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

void DrawEye( HDC hDC, INT Xc, INT Yc, INT w, INT h)
{
  /*SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));*/
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  E(0, 0, w/2, 2);
  E(w/2, 0, w, h);
    l = sqrt((pt.x - rc.right / 2) * (pt.x - rc.right / 2)
             + (pt.y - rc.bottom / 2) * (pt.y - rc.bottom / 2));
    si = (pt.y - rc.bottom / 2) / l;
    co = (pt.x - rc.right / 2) / l;
    m = rc.bottom / 2 - 30;

    x1 = co * m + rc.right / 2;
    y1 = si * m + rc.bottom / 2;*/

  Ellipse(hDC, Xc, Yc, Xc + 150, Yc + 150);
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam)
{
  HDC hDC;
  PAINTSTRUCT ps;
  RECT rc;
  POINT pt;
  float x1, x2, y1, y2, si, co, l, m;

  GetClientRect(hWnd, &rc);

  switch (Msg)
  {
  case WM_DESTROY:
    KillTimer(hWnd, 7);
    PostQuitMessage(0);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(hDC, RGB(255, 255, 255));
    SetDCBrushColor(hDC, RGB(0, 100, 200));
    Ellipse(hDC, 30, 30, rc.right / 2 - 10, rc.bottom - 30);
    Ellipse(hDC, rc.right / 2 + 10, 30, rc.right - 30, rc.bottom - 30);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    /*l = sqrt((pt.x - rc.right / 2) * (pt.x - rc.right / 2)
             + (pt.y - rc.bottom / 2) * (pt.y - rc.bottom / 2));
    si = (pt.y - rc.bottom / 2) / l;
    co = (pt.x - rc.right / 2) / l;
    m = rc.bottom / 2 - 30;

    x1 = co * m + rc.right / 2;
    y1 = si * m + rc.bottom / 2;*/

    DrawEye(hDC, pt.x, pt.y, 300, 300);
    //DrawEye(hDC, x2, pt.y, rc.right, rc.bottom);

    EndPaint(hWnd, &ps);
    return 0;

  case WM_CREATE:
    SetTimer(hWnd, 7, 50, NULL);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}