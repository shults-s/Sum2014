#include <al.h>
#include <alc.h>
#include <alut.h>

#pragma comment(lib, "alut")
#pragma comment(lib, "openal32")

Unit:
  INT SndBuf[2], SndSrc[2]; /* ������ ��� ����� */

Init:
  INT format;
  UINT size, freq;
  VOID *mem;
  CHAR loop;

  /* ������� ������ */
  alGenBuffers(2, Unit->SndBuf);

  /* ��������� ���� � ����� */
  alutLoadWAVFile("a1.wav", &format, &mem,
    &size, &freq, &loop);
  alBufferData(Unit->SndBuf[0], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

  alutLoadWAVFile("a2.wav", &format, &mem,
    &size, &freq, &loop);
  alBufferData(Unit->SndBuf[1], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

  /* ������� ��������� ����� � ��������������� �� */
  alGenSources(2, Unit->SndSrc);

  alSourcei(Unit->SndSrc[0], AL_BUFFER, Unit->SndBuf[0]); /* ���������� ����� �� ���������� */
  alSourcef(Unit->SndSrc[0], AL_PITCH, 0.5);      /* �������� ���������������: 1.0 - �������*/
  alSourcef(Unit->SndSrc[0], AL_GAIN, 3);          /* ���������: 1.0 � ������� */
  alSourcei(Unit->SndSrc[0], AL_LOOPING, 1);       /* ���� �������: 0 � ���, 1 � ���������� */

  alSourcei(Unit->SndSrc[1], AL_BUFFER, Unit->SndBuf[1]);
  alSourcef(Unit->SndSrc[1], AL_PITCH, 1.5);
  alSourcef(Unit->SndSrc[1], AL_GAIN, 0.10);
  alSourcei(Unit->SndSrc[1], AL_LOOPING, 0);
. . .
Response:
    alSourcePlay(Unit->SndSrc[0]);
. . .
    alSourceStop(Unit->SndSrc[0]);

