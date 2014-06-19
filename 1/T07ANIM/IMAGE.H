/* FILENAME: IMAGE.H
 * PROGRAMMER: EF2
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* struct for picture */
typedef struct tagIMAGE
{
  INT W, H;    /* size of picture */
  HBITMAP hBm; /* load image */
  DWORD *Bits; /* array for pixels */
} IMAGE;

/* Load image function */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

/* Free memory for image function */
VOID ImageFree( IMAGE *Img );

/* Get pixel color function */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

/* Draw picture to screen */
VOID ImageDraw( HDC hDC, IMAGE *Img, INT X, INT Y, DWORD Mode );


#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
