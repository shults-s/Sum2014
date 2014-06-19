/* FILENAME: IMAGE.C
 * PROGRAMMER: EF2
 * PURPOSE: Image handle functions
 * LAST UPDATE: 06.06.2014
 */

#include "image.h"

/* Load image function.
 * ARGUMENTS:
 *   - pointer to picture:
 *       IMAGE *Img;
 *   - name of file:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) result of load (TRUE - seccess).
 */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName )
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
    bmi.biHeight = Img->H;
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

/* Free memory for image function.
 * ARGUMENTS:
 *   - ponter to picture:
 *       IMAGE *Img;
 * RETURNS: None.
 */
VOID ImageFree( IMAGE *Img )
{
  if (Img == NULL)
    return;
  if (Img->hBm != NULL)
    DeleteObject(Img->hBm);
  Img->W = Img->H = 0;
  Img->hBm = NULL;
  Img->Bits = NULL;
} /* End of 'ImageFree' function */

/* Get pixel color function.
 * ARGUMENTS:
 *   - pointer to picture:
 *       IMAGE *Img;
 *   - coordinater of pixel:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) color of pixel.
 */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y )
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

/* Draw picture to screen.
 * ARGUMENTS:
 *   - hDC of screen:
 *       HDC hDC;
 *   - image data pointer:
 *       IMAGE *Img;
 *   - coordinates of picture:
 *       INT X, Y;
 *   - mode of drawing:
 *       DWORD Mode;
 * RETURNS: None.
 */
VOID ImageDraw( HDC hDC, IMAGE *Img, INT X, INT Y, DWORD Mode )
{
  HDC hMemDC = CreateCompatibleDC(hDC);

  SelectObject(hMemDC, Img->hBm);
  StretchBlt(hDC, X, Y, Img->W, Img->H, hMemDC, 0, 0, Img->W, Img->H, Mode);

  DeleteDC(hMemDC);
} /* End of 'ImageDraw' function */

/* END OF 'IMAGE.C' FILE */
