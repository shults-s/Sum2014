/* FILENAME: ANIM.C
 * PROGRAMMER: SS3
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>

#include "anim.h"

/* Системный контекст анимации */
static ss3ANIM SS3_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
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

  /* инициализируем таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
} /* End of 'SS3_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID SS3_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < SS3_Anim.NumOfUnits; i++)
  {
    SS3_Anim.Units[i]->Close(SS3_Anim.Units[i], &SS3_Anim);
    free(SS3_Anim.Units[i]);
  }
  DeleteObject(SS3_Anim.hBmFrame);
  DeleteDC(SS3_Anim.hDC);
  memset(&SS3_Anim, 0, sizeof(ss3ANIM));
} /* End of 'SS3_AnimInit' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
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
} /* End of 'SS3_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID SS3_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  /* Обновление ввода */
  GetKeyboardState(SS3_Anim.Keys);
  for (i = 0; i < 256; i++)
    SS3_Anim.Keys[i] >>= 7;

  /* Обновление таймера */
  SS3_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* Обновление кадра */
  QueryPerformanceCounter(&li);

  /* глобальное время */
  SS3_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  SS3_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (SS3_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    SS3_Anim.DeltaTime = 0;
  }
  else
    SS3_Anim.DeltaTime = SS3_Anim.GlobalDeltaTime;

  SS3_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* вычисляем FPS */
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

//////////////////////////////////////////////////////////////////////
// Джойстик
if ((i = joyGetNumDevs()) > 0)
{
  JOYCAPS jc;

  // получение общей информации о джостике
  if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
  {
    JOYINFOEX ji;

    // получение текущего состояния
    ji.dwSize = sizeof(JOYCAPS);
    ji.dwFlags = JOY_RETURNALL;
    if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
    {
      // Кнопки
      memcpy(SS3_Anim.JButsOld, SS3_Anim.JButs, sizeof(SS3_Anim.JButs));
      for (i = 0; i < 32; i++)
        SS3_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
      for (i = 0; i < 32; i++)
        SS3_Anim.JButsClick[i] = SS3_Anim.JButs[i] && !SS3_Anim.JButsOld[i];

      // Оси
      SS3_Anim.JX = VG4_GET_AXIS_VALUE(X);
      SS3_Anim.JY = VG4_GET_AXIS_VALUE(Y);
      if (jc.wCaps & JOYCAPS_HASZ)
        SS3_Anim.JZ = VG4_GET_AXIS_VALUE(Z);
      if (jc.wCaps & JOYCAPS_HASR)
        SS3_Anim.JR = VG4_GET_AXIS_VALUE(R);
      if (jc.wCaps & JOYCAPS_HASU)
        SS3_Anim.JU = VG4_GET_AXIS_VALUE(U) ;

      // Point-Of-View
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
//////////////////////////////////////////////////////////////////////

  /* рисование объектов */
  for (i = 0; i < SS3_Anim.NumOfUnits; i++)
  {
    SelectObject(SS3_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(SS3_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(SS3_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(SS3_Anim.hDC, RGB(55, 155, 255));
    SS3_Anim.Units[i]->Render(SS3_Anim.Units[i], &SS3_Anim);
  }

  FrameCounter++;
} /* End of 'SS3_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID SS3_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(SS3_Anim.hWnd);
  BitBlt(hDC, 0, 0, SS3_Anim.W, SS3_Anim.H, SS3_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(SS3_Anim.hWnd, hDC);
} /* End of 'SS3_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       ss3UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID SS3_AnimAddUnit( ss3UNIT *Unit )
{
  if (SS3_Anim.NumOfUnits < SS3_MAX_UNITS)
  {
    SS3_Anim.Units[SS3_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &SS3_Anim);
  }
} /* End of 'SS3_AnimAddUnit' function */

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID SS3_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(SS3_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(SS3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    /* для одного монитора:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(SS3_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(SS3_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(SS3_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'SS3_AnimFlipFullScreen' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID SS3_AnimSetPause( BOOL NewPauseFlag )
{
  SS3_Anim.IsPause = NewPauseFlag;
} /* End of 'SS3_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */
