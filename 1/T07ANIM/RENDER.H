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

/* ��� ������������� ���������� ��������� */
typedef struct tagef2UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} ef2UV;

/* ������� ������������� ���������� ��������� */
__inline ef2UV EF2_UVSet( FLT U, FLT V )
{
  ef2UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'EF2_UVSet' function */

/* ��� ������������� ������� */
typedef struct tagef2VERTEX
{
  VEC P;    /* ���������� */
  ef2UV T;  /* ���������� ���������� */
  VEC N;    /* ������� */
  COLOR C;  /* ���� */
} ef2VERTEX;


/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagef2PRIM_TYPE
{
  EF2_PRIM_TRIMESH, EF2_PRIM_GRID
} ef2PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagef2PRIM
{
  ef2PRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,          /* ���������� ������ */
    NumOfI,          /* ���������� �������� */
    GW, GH,          /* ������ ����� */
    Mtl,             /* ����� ��������� */
    Size;            /* ������ ���������� ������ � ������ */
  ef2VERTEX *V;      /* ������ ������ */
  INT *I;            /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],      /* ����� ������ � �������� */
    VertexBuffer;    /* ����� �������� ������ */
} ef2PRIM;

/* ���� ��-��������� */
extern COLOR EF2_DefaultColor;

/* ������� �������� ��������� */
BOOL EF2_PrimCreate( ef2PRIM *P, ef2PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL EF2_PrimCreateSphere( ef2PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* ������� �������� ��������� */
BOOL EF2_PrimCreatePlane( ef2PRIM *P, VEC V1, VEC V2, VEC V3, VEC V4, VEC N, INT TexNum);

/* ������� �������� ��������� */
VOID EF2_PrimFree( ef2PRIM *P );

/* ������� ��������� ��������� */
VOID EF2_PrimDraw( ef2PRIM *P );

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagef2MATERIAL
{
  VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} ef2MATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagef2GEOM
{
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  ef2MATERIAL *Mtls;         /* ������� ���������� */
  ef2PRIM *Prims;            /* ������ ���������� */
} ef2GEOM;

/* ������� ���������� ��������� � ������� */
INT EF2_GeomAddMaterial( ef2GEOM *G,
                         ef2MATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT EF2_GeomAddPrim( ef2GEOM *G, ef2PRIM *Prim );

/* ������� ������������ ��������������� ������� */
VOID EF2_GeomFree( ef2GEOM *G );

/* ������� ��������� */
VOID EF2_GeomDraw( ef2GEOM *G );

/* ������� �������� ��������������� ������� */
BOOL EF2_GeomLoad( ef2GEOM *G, CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
