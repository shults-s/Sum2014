/* FILENAME: IMAGE.H
 * PROGRAMMER: RK2
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 12.06.2014
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"
#include "mth.h"

/* Image struct definition */
typedef struct tagrk2IMAGE
{
  INT W, H;    /* Picture size */
  HBITMAP hBm; /* Descriptor of loaded picture */
  DWORD *Bits; /* Image row */
} rk2IMAGE;

/* Loading picture from file function. */
BOOL ImageLoad( rk2IMAGE *Img, CHAR *FileName );

/* Picture memory free function. */
VOID ImageFree( rk2IMAGE *Img );

/* Getting image point color function. */
DWORD ImageGetP( rk2IMAGE *Img, INT X, INT Y );

/* Image Blit to hDC function. */
VOID ImageDraw( HDC hDC, rk2IMAGE *Img, DWORD Rop, INT X0, INT Y0, INT X1, INT Y1 );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
