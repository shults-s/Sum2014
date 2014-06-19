/* FILENAME: ANIM.H
 * PROGRAMMER: EF2
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "render.h"
#include "image.h"
#include "shader.h"

/* maximum of units in animation */
#define EF2_MAX_UNITS 3000

/* Declaration of unit */
typedef struct tagef2UNIT ef2UNIT;

/* Structure for animation */
typedef struct tagef2ANIM
{
  HWND hWnd;          /* Output screen */
  INT W, H;           /* Size of screen */
  HDC hDC;            /* hDC of screen */
  HGLRC hRC;           /* context of buiding screen */

  /* Arrey of units in animation */
  ef2UNIT *Units[EF2_MAX_UNITS]; 
  INT NumOfUnits;

  /* Data for sinchronize */
  DBL
    Time,            /* time in seconds from begin */
    GlobalTime,      /* time without pause */
    DeltaTime,       /* time between frames */
    GlobalDeltaTime, /* time between frames without pause */
    FPS;             /* Frames per second */
  BOOL
    IsPause;         /* flag in pause */

  /* Input system */
  BYTE
    Keys[256],       /* Information about keys in keybord */
    KeysOld[256],    /* Information about keys in frame */
    KeysClick[256];  /* Information about once key ckick */
  INT
    MsDeltaX, MsDeltaY, /* Otnos coord */
    MsX, MsY,           /* Abs coord */
    MsWheel,            /* Information about mouse wheel */
    CountX, CountY;     /* Cordinates for camera */
  BYTE
    JButs[32],       /* Information of joy keys */
    JButsOld[32],    /* Information of joy keys in frame */
    JButsClick[32],  /* Information about once joy button click */
    JPOV;            /* POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU; /* Osi of joy */

  /* Render parametrs */
  DBL
    Wp, Hp,          /* Size of view */
    ProjDist,        /* Project distance */
    ProjSize;        /* Soze of projcetion */

  /* Matrix */
  MATR
    MatrWorld,       /* Matrix of global SK */
    MatrView,        /* Matrix of lokal SK */
    MatrProjection;  /* Projection matrix */
  /* Camera */
  VEC PosCam; /* Position of camera vector data */
} ef2ANIM;

extern ef2ANIM EF2_Anim;

/* Base fields of unit:
 * - size of unit
 *     INT Size;
 * - pointer for init
 *     VOID (*Init)( ef2UNIT *Unit, ef2ANIM *Ani );
 * - pointer for uninit
 *     VOID (*Close)( ef2UNIT *Unit, ef2ANIM *Ani );
 * - pointer for response
 *     VOID (*Response)( ef2UNIT *Unit, ef2ANIM *Ani );
 * - pointer for render
 *     VOID (*Render)( ef2UNIT *Unit, ef2ANIM *Ani );
 */
#define EF2_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ef2UNIT *Unit, ef2ANIM *Ani );       \
  VOID (*Close)( ef2UNIT *Unit, ef2ANIM *Ani );      \
  VOID (*Response)( ef2UNIT *Unit, ef2ANIM *Ani );   \
  VOID (*Render)( ef2UNIT *Unit, ef2ANIM *Ani )

/* Main type of unit */
struct tagef2UNIT
{
  EF2_UNIT_BASE_FIELDS; /* Base fields */
};

extern UINT EF2_ShaderProg;
/***
 * Work with animation module of function
 ***/

/* Init of animation function */
BOOL EF2_AnimInit( HWND hWnd );

/* Uninit of animation function */
VOID EF2_AnimClose( VOID );

/* Resize of animation function */
VOID EF2_AnimResize( INT W, INT H );

/* Render of animation function */
VOID EF2_AnimRender( VOID );

/* Output anumation function */
VOID EF2_AnimCopyFrame( VOID );

/* do fullscreen to animation function */
VOID EF2_AnimFlipFullScreen( VOID );

/* Set pause in animation function */
VOID EF2_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Add unit to animation function */
VOID EF2_AnimAddUnit( ef2UNIT *Unit );

/* Create unit function */
ef2UNIT * EF2_AnimUnitCreate( INT Size );

ef2UNIT * EF2_PlansUnitCreate( VOID );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
