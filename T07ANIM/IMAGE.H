/* FILENAME: IMAGE.H
 * PROGRAMMER: SS3
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

typedef struct tagIMAGE
{
  INT W, H;
  HBITMAP hBm;
  DWORD *Bits;
} IMAGE;

BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

VOID ImageFree( IMAGE *Img );

DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

VOID ImageDraw( HDC hDC, IMAGE *Img, INT X, INT Y, DWORD Mode );

#endif