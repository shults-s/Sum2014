
/* ������ ��� ��������� ���� */
static INT
  VG4_MouseGlobalX, VG4_MouseGlobalY, /* ������� ���������� */
  VG4_MouseXOld, VG4_MouseYOld,       /* ����������� �� �������� ����� ���������� */
  VG4_MouseGlobalWheel;               /* ��������� ������ ���� */

/* ������� ��������� ����������� �������� ���������.
 * ���������:
 *   - ��� �������:
 *       INT Code;
 *   - �������� ��������� ('word parameter') -
 *     ��� ��������� �� ����:
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter') -
 *     (MSLLHOOKSTRUCT *) ��� �������� ���������;
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
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



  /* �������������� ������ ��������� �� ���� */
  SetWindowsHookEx(WH_MOUSE_LL, VG4_MouseHook, GetModuleHandle(NULL), 0);


  /* ���� */
  /*  ������ */
  VG4_Anim.MsWheel = VG4_MouseGlobalWheel;
  VG4_MouseGlobalWheel = 0;
  /* ���������� ������� */
  pt.x = VG4_MouseGlobalX;
  pt.y = VG4_MouseGlobalY;
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_Anim.MsX = pt.x;
  VG4_Anim.MsY = pt.y;
  /* ������������� ����������� */
  VG4_Anim.MsDeltaX = VG4_MouseGlobalX - VG4_MouseXOld;
  VG4_Anim.MsDeltaY = VG4_MouseGlobalY - VG4_MouseYOld;
  VG4_MouseXOld = VG4_MouseGlobalX;
  VG4_MouseYOld = VG4_MouseGlobalY;
