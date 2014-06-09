  /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(VG4_Anim.JButsOld, VG4_Anim.JButs, sizeof(VG4_Anim.JButs));
        for (i = 0; i < 32; i++)
          VG4_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          VG4_Anim.JButsClick[i] = VG4_Anim.JButs[i] && !VG4_Anim.JButsOld[i];

        /* Оси */
        VG4_Anim.JX = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin - 1) - 1;
        VG4_Anim.JY = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASZ)
          VG4_Anim.JZ = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASR)
          VG4_Anim.JR = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASU)
          VG4_Anim.JU = 2.0 * (ji.dwUpos - jc.wUmin) / (jc.wUmax - jc.wUmin - 1) - 1;

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            VG4_Anim.JPOV = 0;
          else
            VG4_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

