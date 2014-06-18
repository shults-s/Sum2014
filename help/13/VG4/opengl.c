_CRT_SECURE_NO_WARNINGS

E:\$(UserName)\$(ProjectName)\$(ConfigurationName)

����� �����������:

#pragma warning(disable: 4244 4305)

#include <windows.h>
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <mmsystem.h>

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32")
#pragma comment(lib, "winmm")

/* Large integer data types */
typedef __int64 INT64;
typedef unsigned __int64 UINT64;

/* Common float point types */
typedef float FLT;
typedef double DBL;


�������������:
  PIXELFORMATDESCRIPTOR pfd = {0};

  /*** ������������� OpenGL ***/

  /* ��������� ������ ����� */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(VG4_Anim.hDC, &pfd);
  DescribePixelFormat(VG4_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(VG4_Anim.hDC, i, &pfd);

  /* ������� �������� ���������� */
  VG4_Anim.hRC = wglCreateContext(VG4_Anim.hDC);
  /* ������ �������� ��������� */
  wglMakeCurrent(VG4_Anim.hDC, VG4_Anim.hRC);

  /* �������������� ���������� */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(VG4_Anim.hRC);
    ReleaseDC(VG4_Anim.hWnd, VG4_Anim.hDC);
    memset(&VG4_Anim, 0, sizeof(vg4ANIM));
    return FALSE;
  }
  /* ��������� OpenGL ��-��������� */
  . . .

� ������� ��������� ��������� ����:
  case WM_CREATE:
    . . .
    if (!VG4_AnimInit(hWnd))
      return -1;
    return 0;
    . . .

���������������:
  . . .
  /* ��������� ��������� */
  wglMakeCurrent(NULL, NULL);

  wglDeleteContext(VG4_Anim.hRC);
  ReleaseDC(VG4_Anim.hWnd, VG4_Anim.hDC);
  . . .

��������� ������� ������� ������:
  . . .
  /* ���� ��������� */
  glViewport(0, 0, W, H);
  . . .

�����:
  - �������
  /* ������� ���� */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  - ���������
  glRectd(0, 0, 1, 1);

  - ����� �����
  glFinish();
  SwapBuffers(VG4_Anim.hDC);


//������ ���������:

  Ani->MatrWorld = MatrRotateY(30 * sin(Ani->Time));
  Ani->MatrView = MatrViewLookAt(VecSet(15, 15, 15), VecSet(0, 0, 0), VecSet(0, 1, 0));


//���������� ���������:
  DBL ratio_x = 1, ratio_y = 1;

  /* ���������� ������� */
  VG4_Anim.W = W;
  VG4_Anim.H = H;
  /* ���� ��������� */
  glViewport(0, 0, W, H);

  if (W > H)
    ratio_x = (DBL)W / H;
  else
    ratio_y = (DBL)H / W;
  VG4_Anim.Wp = VG4_Anim.ProjSize * ratio_x;
  VG4_Anim.Hp = VG4_Anim.ProjSize * ratio_y;

����� ������� - �������� ������:
  ����� ���� ��� ��� ��������� �������:
  VG4_Anim.MatrProjection =
      MatrProjection(-VG4_Anim.Wp / 2, VG4_Anim.Wp / 2,
                     -VG4_Anim.Hp / 2, VG4_Anim.Hp / 2,
                      VG4_Anim.ProjDist, VG4_Anim.FarClip);

  //������ ��� ��� ��������� �������:
  Ani->MatrWorld = MatrTranslate(0, 0, 9 * sin(Ani->Time));
  Ani->MatrView = MatrViewLookAt(VecSet(15, 15, 15), VecSet(0, 0, 0), VecSet(0, 1, 0));

  //�����������
  MATR WVP;
  WVP = MatrMulMatr(Ani->MatrWorld,
    MatrMulMatr(Ani->MatrView, Ani->MatrProjection));

  //��������� � OpenGL:
  glLoadMatrixd(WVP.A[0]);

  //�������:

  glBegin(GL_LINES);
    glColor3d(1, 0.5, 0.5);
    glVertex3d(-3, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0.5, 1, 0.5);
    glVertex3d(0, -3, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0.5, 0.5, 1);
    glVertex3d(0, 0, -3);
    glVertex4d(0, 0, 1, 0);
  glEnd();








