- ������:

    ��������:
glColorMask(r, g, b, a); - ���������� ������ 0|1  false|true
glClearColor(r, g, b, a);  0..1
glClear(GL_COLOR_BUFFER_BIT);

    �������:
glDepthMask(d); - ���������� ������ 0|1  false|true
glClearDepth(d_cl); [-1..1]
glDepthRange(near, far);
glEnable(GL_DEPTH_TEST); - ��������� �������� �������
glClear(GL_DEPTH_BUFFER_BIT);

    �����-����� � ������������ �����:
glAlphaFunc(�������, ref);
  	GL_GREATER -->  alpha > ref
glEnable(GL_ALPHA_TEST);
-->����������
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

