#include <al.h>
#include <alc.h>
#include <alut.h>

#pragma comment(lib, "alut")
#pragma comment(lib, "openal32")

Unit:
  INT SndBuf[2], SndSrc[2]; /* данные дл€ звука */

Init:
  INT format;
  UINT size, freq;
  VOID *mem;
  CHAR loop;

  /* создаем буфера */
  alGenBuffers(2, Unit->SndBuf);

  /* загружаем звук в буфер */
  alutLoadWAVFile("a1.wav", &format, &mem,
    &size, &freq, &loop);
  alBufferData(Unit->SndBuf[0], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

  alutLoadWAVFile("a2.wav", &format, &mem,
    &size, &freq, &loop);
  alBufferData(Unit->SndBuf[1], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

  /* создаем источники звука и параметризируем их */
  alGenSources(2, Unit->SndSrc);

  alSourcei(Unit->SndSrc[0], AL_BUFFER, Unit->SndBuf[0]); /* закрепл€ем буфер за источником */
  alSourcef(Unit->SndSrc[0], AL_PITCH, 0.5);      /* скорость воспроизведени€: 1.0 - обычна€*/
  alSourcef(Unit->SndSrc[0], AL_GAIN, 3);          /* громкость: 1.0 Ц обычна€ */
  alSourcei(Unit->SndSrc[0], AL_LOOPING, 1);       /* флаг повтора: 0 Ц нет, 1 Ц бесконечно */

  alSourcei(Unit->SndSrc[1], AL_BUFFER, Unit->SndBuf[1]);
  alSourcef(Unit->SndSrc[1], AL_PITCH, 1.5);
  alSourcef(Unit->SndSrc[1], AL_GAIN, 0.10);
  alSourcei(Unit->SndSrc[1], AL_LOOPING, 0);
. . .
Response:
    alSourcePlay(Unit->SndSrc[0]);
. . .
    alSourceStop(Unit->SndSrc[0]);

