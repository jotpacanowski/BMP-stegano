#pragma once
#include "bmp_headers.h"
// $ python ./generate-bmp-dumping.py
static inline void dump_file_header(BITMAPFILEHEADER* x){
    printf("BITMAPFILEHEADER\n");
    printf("              bfType : %#6hx  %hu\n", x->bfType, x->bfType);
    printf("              bfSize : %#10x  %u\n", x->bfSize, x->bfSize);
    printf("         bfReserved1 : %#6hx  %hu\n", x->bfReserved1, x->bfReserved1);
    printf("         bfReserved2 : %#6hx  %hu\n", x->bfReserved2, x->bfReserved2);
    printf("           bfOffBits : %#10x  %u\n", x->bfOffBits, x->bfOffBits);
}

static inline void dump_info_header(BITMAPINFOHEADER* x){
    printf("BITMAPINFOHEADER\n");
    printf("              biSize : %#10x  %u\n", x->biSize, x->biSize);
    printf("             biWidth : %#10x  %d\n", x->biWidth, x->biWidth);
    printf("            biHeight : %#10x  %d\n", x->biHeight, x->biHeight);
    printf("            biPlanes : %#6hx  %hu\n", x->biPlanes, x->biPlanes);
    printf("          biBitCount : %#6hx  %hu\n", x->biBitCount, x->biBitCount);
    printf("       biCompression : %#10x  %u\n", x->biCompression, x->biCompression);
    printf("         biSizeImage : %#10x  %u\n", x->biSizeImage, x->biSizeImage);
    printf("     biXPelsPerMeter : %#10x  %d\n", x->biXPelsPerMeter, x->biXPelsPerMeter);
    printf("     biYPelsPerMeter : %#10x  %d\n", x->biYPelsPerMeter, x->biYPelsPerMeter);
    printf("           biClrUsed : %#10x  %u\n", x->biClrUsed, x->biClrUsed);
    printf("      biClrImportant : %#10x  %u\n", x->biClrImportant, x->biClrImportant);
    if(x->biSize < 108)
        return;
    printf("          bV4RedMask : %#10x  %u\n", x->bV4RedMask, x->bV4RedMask);
    printf("        bV4GreenMask : %#10x  %u\n", x->bV4GreenMask, x->bV4GreenMask);
    printf("         bV4BlueMask : %#10x  %u\n", x->bV4BlueMask, x->bV4BlueMask);
    printf("        bV4AlphaMask : %#10x  %u\n", x->bV4AlphaMask, x->bV4AlphaMask);
    printf("           bV4CSType : %#10x  %u\n", x->bV4CSType, x->bV4CSType);
    printf("        bV4Endpoints : ???\n");
    printf("         bV4GammaRed : %#10x  %u\n", x->bV4GammaRed, x->bV4GammaRed);
    printf("       bV4GammaGreen : %#10x  %u\n", x->bV4GammaGreen, x->bV4GammaGreen);
    printf("        bV4GammaBlue : %#10x  %u\n", x->bV4GammaBlue, x->bV4GammaBlue);
    if(x->biSize < 124)
        return;
    printf("           bV5Intent : %#10x  %u\n", x->bV5Intent, x->bV5Intent);
    printf("      bV5ProfileData : %#10x  %u\n", x->bV5ProfileData, x->bV5ProfileData);
    printf("      bV5ProfileSize : %#10x  %u\n", x->bV5ProfileSize, x->bV5ProfileSize);
    printf("         bV5Reserved : %#10x  %u\n", x->bV5Reserved, x->bV5Reserved);
}
