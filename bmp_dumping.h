#pragma once
#include "bmp_headers.h"
// $ python ./generate-bmp-dumping.py > bmp_dumping.h
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
}

