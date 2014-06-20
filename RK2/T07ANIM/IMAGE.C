/* FILENAME: IMAGE.C
 * PROGRAMMER: RK2
 * PURPOSE: Image handle functions
 * LAST UPDATE: 12.06.2014
 */

#include "image.h"

/* Loading picture from file function.
 * ARGUMENTS:
 *   - Pointer to new picture:
 *       rk2IMAGE *Img;
 *   - File Name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) result (TRUE - done).
 */
BOOL ImageLoad( rk2IMAGE *Img, CHAR *FileName )
{
  HBITMAP hBmLoad;

  if (Img == NULL)
    return FALSE;
  Img->W = Img->H = 0;
  Img->hBm = NULL;
  Img->Bits = NULL;
  if ((hBmLoad = LoadImage(NULL, FileName,
         IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) != NULL)
  {
    BITMAP bm;
    BITMAPINFOHEADER bmi;
    HDC hDC, hMemDC1, hMemDC2;

    GetObject(hBmLoad, sizeof(bm), &bm);
    Img->W = bm.bmWidth;
    Img->H = bm.bmHeight;

    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biWidth = Img->W;
    bmi.biHeight = -Img->H;
    bmi.biBitCount = 32;
    bmi.biPlanes = 1;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = Img->W * Img->H * 4;
    bmi.biXPelsPerMeter = bmi.biYPelsPerMeter = 1;
    bmi.biClrUsed = 0;
    bmi.biClrImportant = 0;

    Img->hBm = CreateDIBSection(NULL, (BITMAPINFO *)&bmi, DIB_RGB_COLORS,
      (VOID **)&Img->Bits, NULL, 0);

    hDC = GetDC(NULL);
    hMemDC1 = CreateCompatibleDC(hDC);
    hMemDC2 = CreateCompatibleDC(hDC);

    SelectObject(hMemDC1, hBmLoad);
    SelectObject(hMemDC2, Img->hBm);
    BitBlt(hMemDC2, 0, 0, Img->W, Img->H, hMemDC1, 0, 0, SRCCOPY);

    DeleteDC(hMemDC1);
    DeleteDC(hMemDC2);
    ReleaseDC(NULL, hDC);
    DeleteObject(hBmLoad);
  }

  return Img->hBm != NULL;
} /* End of 'ImageLoad' function */

/* Free image memory function.
 * ARGUMENTS:
 *   - pointer for picture:
 *       rk2IMAGE *Img;
 * RETURNS: None.
 */
VOID ImageFree( rk2IMAGE *Img )
{
  if (Img == NULL)
    return;
  if (Img->hBm != NULL)
    DeleteObject(Img->hBm);
  Img->W = Img->H = 0;
  Img->hBm = NULL;
  Img->Bits = NULL;
} /* End of 'ImageFree' function */

/* Getting imager point color function.
 * ARGUMENTS:
 *   - Pointer for image::
 *       rk2IMAGE *Img;
 *   - Position of point:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) Color of point.
 */
DWORD ImageGetP( rk2IMAGE *Img, INT X, INT Y )
{
  if (Img == NULL)
    return 0;
  if (Img->hBm != NULL &&
      X >= 0 && Y >= 0 && X < Img->W && Y < Img->H)
  {
    DWORD color;
    BYTE r, g, b;

    color = Img->Bits[Y * Img->W + X];
    b = color & 0xFF;
    g = (color >> 8) & 0xFF;
    r = (color >> 16) & 0xFF;

    return RGB(r, g, b);
  }
  return 0;
} /* End of 'ImageGetP' function */

/* Image Blit to hDC function.
 * ARGUMENTS:
 *   - Destination hDC:
 *      HDC hDestDC
 *   - Pointer for image:
 *       rk2IMAGE *Img;
 *   - RECT position on window:
 *       INT X0, Y0, X1, Y1;
 * RETURNS:
 *   (VOID) None.
 */
VOID ImageDraw( HDC hDC, rk2IMAGE *Img, DWORD Rop, INT X0, INT Y0, INT X1, INT Y1 )
{
  /* HDC hDC = GetDC(hWnd); */
  HDC hMemDC = CreateCompatibleDC(hDC);

  /* Blit picture */
  SelectObject(hMemDC, Img->hBm);
  StretchBlt(hDC, X0, Y0, Img->W, Img->H, hMemDC, 0, 0, Img->W, Img->H, Rop);

  /* Free tmp hDC */
  DeleteDC(hMemDC);
} /* End of 'ImageDraw' function */

/* END OF 'IMAGE.C' FILE */
