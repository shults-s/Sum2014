/* FILENAME: ANIM.H
 * PROGRAMMER: SS3
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Максимальное количество элементов анимации */
#define SS3_MAX_UNITS 3000

/* Точность вычисления позиции джойстика */
#define VG4_JOYSTICK_THRESHOLD 64.0

/* Устранение ошибок замера джойстика */
#define VG4_GET_AXIS_VALUE(A) \
  ((INT)(VG4_JOYSTICK_THRESHOLD * (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / \
   (jc.w ## A ## max - jc.w ## A ##min - 1) - 1) + 0.5) / VG4_JOYSTICK_THRESHOLD)

/* Объявление типа объектов анимации "вперед" */
typedef struct tagss3UNIT ss3UNIT;

/* Структура хранения контекста анимации */
typedef struct tagss3ANIM
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */

  /* Массив элементов анимации и их количество */
  ss3UNIT *Units[SS3_MAX_UNITS]; 
  INT NumOfUnits;

  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Подсистема ввода */
  BYTE Keys[256], JButsOld[32], JButs[32], JButsClick[32];
  DBL JX, JY, JZ, JR, JU, JPOV;     /* Сотояние клавиш клавиатуры и мыши */
} ss3ANIM;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - указатель на функцию инициализации
 *     VOID (*Init)( ss3UNIT *Unit, ss3ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( ss3UNIT *Unit, ss3ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( ss3UNIT *Unit, ss3ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( ss3UNIT *Unit, ss3ANIM *Ani );
 */
#define SS3_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ss3UNIT *Unit, ss3ANIM *Ani );       \
  VOID (*Close)( ss3UNIT *Unit, ss3ANIM *Ani );      \
  VOID (*Response)( ss3UNIT *Unit, ss3ANIM *Ani );   \
  VOID (*Render)( ss3UNIT *Unit, ss3ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagss3UNIT
{
  SS3_UNIT_BASE_FIELDS; /* Базовые поля */
};

/***
 * Функции управления анимацией
 ***/

/* Функция инициализации анимации */
VOID SS3_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID SS3_AnimClose( VOID );

/* Функция обработки изменения размера области вывода */
VOID SS3_AnimResize( INT W, INT H );

/* Функция построения кадра анимации */
VOID SS3_AnimRender( VOID );

/* Функция вывода кадра анимации */
VOID SS3_AnimCopyFrame( VOID );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов */
VOID SS3_AnimFlipFullScreen( VOID );

/* Функция установки паузы анимации */
VOID SS3_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Функция добавления в систему объекта анимации */
VOID SS3_AnimAddUnit( ss3UNIT *Unit );

/* Функция создания объекта анимации */
ss3UNIT * SS3_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
