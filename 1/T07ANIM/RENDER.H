/* FILENAME: RENDER.H
 * PROGRAMMER: EF2
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "matr.h"

/* Type of declaration camera in world */
typedef struct
{
  VEC
    Loc,             /* Coordinates */
    Dir, Up, Right,  /* Orientation */
    At;              /* Point of view */
} ef2CAMERA;

/* Gometric object data type */
typedef struct tagef2GOBJ
{
  VEC *V;      /* array for vectors */
  INT NumOfV;  /* number of vectors */
  INT (*F)[3]; /* array for  */
  INT NumOfF;  /* number of  */
} ef2GOBJ;

/* Load geometric object function */
BOOL EF2_RndGObjLoad( ef2GOBJ *GObj, CHAR *FileName );

/* Free memory from geometric object function */
VOID EF2_RndGObjFree( ef2GOBJ *GObj );

/* Draw geometric object function */
VOID EF2_RndGObjDraw( ef2GOBJ *GObj, HDC hDC );


/***
 * Geometric object module of function
 ***/

/* Тип представления текстурных координат */
typedef struct tagef2UV
{
  FLT U, V; /* компоненты текстурных координат */
} ef2UV;

/* Функиця инициализации текстурных координат */
__inline ef2UV EF2_UVSet( FLT U, FLT V )
{
  ef2UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'EF2_UVSet' function */

/* Теп представления вершины */
typedef struct tagef2VERTEX
{
  VEC P;    /* координаты */
  ef2UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} ef2VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagef2PRIM_TYPE
{
  EF2_PRIM_TRIMESH, EF2_PRIM_GRID
} ef2PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagef2PRIM
{
  ef2PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,          /* количество вершин */
    NumOfI,          /* количество индексов */
    GW, GH,          /* размер сетки */
    Mtl,             /* номер материала */
    Size;            /* размер выделенной памяти в байтах */
  ef2VERTEX *V;      /* массив вершин */
  INT *I;            /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],      /* Буфер вершин и индексов */
    VertexBuffer;    /* Буфер описания вершин */
} ef2PRIM;

/* Цвет по-умолчанию */
extern COLOR EF2_DefaultColor;

/* Функция создания примитива */
BOOL EF2_PrimCreate( ef2PRIM *P, ef2PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL EF2_PrimCreateSphere( ef2PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* Функция создания плоскости */
BOOL EF2_PrimCreatePlane( ef2PRIM *P, VEC V1, VEC V2, VEC V3, VEC V4, VEC N, INT TexNum);

/* Функция удаления примитива */
VOID EF2_PrimFree( ef2PRIM *P );

/* Функция отрисовки примитива */
VOID EF2_PrimDraw( ef2PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagef2MATERIAL
{
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} ef2MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagef2GEOM
{
  INT NumOfPrims, NumOfMtls; /* размерв массивов */
  ef2MATERIAL *Mtls;         /* таблица материалов */
  ef2PRIM *Prims;            /* массив примитивов */
} ef2GEOM;

/* Функция добавления материала к объекту */
INT EF2_GeomAddMaterial( ef2GEOM *G,
                         ef2MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT EF2_GeomAddPrim( ef2GEOM *G, ef2PRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID EF2_GeomFree( ef2GEOM *G );

/* Функция рисования */
VOID EF2_GeomDraw( ef2GEOM *G );

/* Функция загрузки геометрического объекта */
BOOL EF2_GeomLoad( ef2GEOM *G, CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
