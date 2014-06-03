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

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 7, 30, NULL);
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
    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDCClock, 0, 0, SRCCOPY);

    MoveToEx(hMemDC, 175, 175, NULL);
    LineTo(hMemDC, 270 - cc, 270);

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