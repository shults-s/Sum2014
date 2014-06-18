- буфера:

    цветовой:
glColorMask(r, g, b, a); - разрешение записи 0|1  false|true
glClearColor(r, g, b, a);  0..1
glClear(GL_COLOR_BUFFER_BIT);

    глубины:
glDepthMask(d); - разрешение записи 0|1  false|true
glClearDepth(d_cl); [-1..1]
glDepthRange(near, far);
glEnable(GL_DEPTH_TEST); - включение проверки глубины
glClear(GL_DEPTH_BUFFER_BIT);

    альфа-канал и интерпол€ци€ цвета:
glAlphaFunc(функци€, ref);
  	GL_GREATER -->  alpha > ref
glEnable(GL_ALPHA_TEST);
-->смешивание
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

