
/* Данные для обработки мыши */
static INT
  VG4_MouseGlobalX, VG4_MouseGlobalY, /* Текущие координаты */
  VG4_MouseXOld, VG4_MouseYOld,       /* Сохраненные от прошлого кадра координаты */
  VG4_MouseGlobalWheel;               /* Состояние колеса мыши */

/* Функция обработки захваченных мышинных сообщений.
 * АРГУМЕНТЫ:
 *   - код захвата:
 *       INT Code;
 *   - параметр сообшения ('word parameter') -
 *     код сообщения от мыши:
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter') -
 *     (MSLLHOOKSTRUCT *) для мышинных сообщений;
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
static LRESULT CALLBACK VG4_MouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    VG4_MouseGlobalX = hs->pt.x;
    VG4_MouseGlobalY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    VG4_MouseGlobalWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return 0;
} /* End of 'VG4_MouseHook' function */



  /* инициализируем захват сообщений от мыши */
  SetWindowsHookEx(WH_MOUSE_LL, VG4_MouseHook, GetModuleHandle(NULL), 0);


  /* Мышь */
  /*  колесо */
  VG4_Anim.MsWheel = VG4_MouseGlobalWheel;
  VG4_MouseGlobalWheel = 0;
  /* абсолютная позиция */
  pt.x = VG4_MouseGlobalX;
  pt.y = VG4_MouseGlobalY;
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_Anim.MsX = pt.x;
  VG4_Anim.MsY = pt.y;
  /* относительное перемещение */
  VG4_Anim.MsDeltaX = VG4_MouseGlobalX - VG4_MouseXOld;
  VG4_Anim.MsDeltaY = VG4_MouseGlobalY - VG4_MouseYOld;
  VG4_MouseXOld = VG4_MouseGlobalX;
  VG4_MouseYOld = VG4_MouseGlobalY;
