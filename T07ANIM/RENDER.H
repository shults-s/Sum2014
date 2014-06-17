/* FILENAME: RENDER.H
 * PROGRAMMER: SS3
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

typedef struct
{
  VEC
    Loc,
    Dir, Up, Right,
    At;
} ss3CAMERA;

typedef struct tagss3GOBJ
{
  VEC *V;
  INT NumOfV;
  INT (*F)[3];
  INT NumOfF;
} ss3GOBJ;

BOOL SS3_RndGObjLoad( ss3GOBJ *GObj, CHAR *FileName );

VOID SS3_RndGObjFree( ss3GOBJ *GObj );

VOID SS3_RndGObjDraw( ss3GOBJ *GObj, HDC hDC );

typedef struct tagss3UV
{
  FLT U, V; /* компоненты текстурных координат */
} ss3UV;

/* Функиця инициализации текстурных координат */
__inline ss3UV SS3_UVSet( FLT U, FLT V )
{
  ss3UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
}

/* Теп представления вершины */
typedef struct tagss3VERTEX
{
  VEC P;    /* координаты */
  ss3UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} ss3VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagss3PRIM_TYPE
{
  SS3_PRIM_TRIMESH, SS3_PRIM_GRID
} ss3PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagss3PRIM
{
  ss3PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,     /* количество вершин */
    NumOfI,     /* количество индексов */
    GW, GH,     /* размер сетки */
    Mtl,        /* номер материала */
    Size;       /* размер выделенной памяти в байтах */
  ss3VERTEX *V; /* массив вершин */
  INT *I;       /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
} ss3PRIM;

/* Цвет по-умолчанию */
extern COLOR SS3_DefaultColor;

/* Функция создания примитива */
BOOL SS3_PrimCreate( ss3PRIM *P, ss3PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL SS3_PrimCreateSphere( ss3PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* Функция удаления примитива */
VOID SS3_PrimFree( ss3PRIM *P );

/* Функция отрисовки примитива */
VOID SS3_PrimDraw( ss3PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagss3MATERIAL
{
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} ss3MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagss3GEOM
{
  INT NumOfPrims, NumOfMtls; /* размерв массивов */
  ss3MATERIAL *Mtls;         /* таблица материалов */
  ss3PRIM *Prims;            /* массив примитивов */
} ss3GEOM;

/* Функция добавления материала к объекту */
INT SS3_GeomAddMaterial( ss3GEOM *G,
                         ss3MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT SS3_GeomAddPrim( ss3GEOM *G, ss3PRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID SS3_GeomFree( ss3GEOM *G );

/* Функция рисования */
VOID SS3_GeomDraw( ss3GEOM *G );

BOOL SS3_GeomLoad( ss3GEOM *G, CHAR *FileName );

#endif