#pragma once
#include <stdint.h>

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

#pragma pack(push,1)

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-ciexyz
// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-ciexyztriple
typedef int32_t FXPT2DOT30;
typedef struct tagCIEXYZ {
  FXPT2DOT30 ciexyzX;
  FXPT2DOT30 ciexyzY;
  FXPT2DOT30 ciexyzZ;
} CIEXYZ;
typedef struct tagCIEXYZTRIPLE {
  CIEXYZ ciexyzRed;
  CIEXYZ ciexyzGreen;
  CIEXYZ ciexyzBlue;
} CIEXYZTRIPLE;

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

// https://docs.microsoft.com/pl-pl/previous-versions/dd183376(v=vs.85)
// https://docs.microsoft.com/pl-pl/windows/win32/api/wingdi/ns-wingdi-bitmapv4header
//
typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
  // -- version 4
  DWORD        bV4RedMask;
  DWORD        bV4GreenMask;
  DWORD        bV4BlueMask;
  DWORD        bV4AlphaMask;
  DWORD        bV4CSType;
  CIEXYZTRIPLE bV4Endpoints;
  DWORD        bV4GammaRed;
  DWORD        bV4GammaGreen;
  DWORD        bV4GammaBlue;
  // -- version 5
  DWORD        bV5Intent;
  DWORD        bV5ProfileData;
  DWORD        bV5ProfileSize;
  DWORD        bV5Reserved;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

#pragma pack(pop)
