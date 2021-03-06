// 2022 Jędrzej Pacanowski
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp_dumping.h"
#include "bmp_headers.h"
#include "histogram.h"
#include "steganography.h"

BITMAPINFOHEADER infoh;
BITMAPFILEHEADER fileh;

void verify_fileh(BITMAPFILEHEADER* x)
{
    if (x->bfType != 0x424d)  // byte-swapped "BM"
        puts("File magic is OK (BM)");
    else
        puts("Wrong file magic");

    if (x->bfReserved1 != 0 || x->bfReserved2 != 0) puts("Reserved fields are not zero!");
}

void verify_infoh(BITMAPINFOHEADER* x)
{
    if (x->biSize == 40) puts("INFO header biSize is 40 bytes");
    if (x->biSize == 108) puts("INFO header is v4 - 108 bytes");
    if (x->biSize == 124)
        puts("INFO header is v5 - 124 bytes");
    else
        puts("INFO header biSize is not in [40, 108, 124]");

    if (x->biPlanes != 1) puts("biPlanes is not 1");

    if (x->biSize >= 108) {
        if (x->bV4CSType == 0x73524742)  // LCS_sRGB
            puts("sRGB color space");
        else
            puts("Color space is not \'sRGB\'");
    }
}

void convert_to_grayscale(FILE* f, unsigned char* row, size_t row_len);

int main(int argc, char** argv)
{
    if (argc < 2) {
        puts("wrong/no filename");
        return 2;
    }

    FILE* f = fopen(argv[1], "rb");
    if (f == NULL) {
        perror("Failed to open file");
        return 1;
    }

    if (fread(&fileh, sizeof(BITMAPFILEHEADER), 1, f) != 1) {
        perror("Failed to read FILE header");
        fclose(f);
        return 1;
    }

    if (argc == 2) {
        dump_file_header(&fileh);
        puts("");
    }

    if (fread(&infoh, sizeof(BITMAPINFOHEADER), 1, f) != 1) {
        perror("Failed to read INFO header");
        fclose(f);
        return 1;
    }

    if (argc == 2) {
        dump_info_header(&infoh);
        puts(" - - - ");
    }

    verify_fileh(&fileh);
    verify_infoh(&infoh);

    if (infoh.biCompression != 0) {
        printf("Unsupported compression algorithm %u\n", infoh.biCompression);
        fclose(f);
        return 1;
    }
    if (infoh.biBitCount != 24) {
        printf("Unsupported bitmap: bitCount = %u\n", infoh.biBitCount);
        fclose(f);
        return 1;
    }

    if (fileh.bfOffBits != sizeof(BITMAPFILEHEADER) + infoh.biSize) {
        printf("Possible hidden data after headers (size is %zu, offset is %u)\n",
               sizeof(BITMAPFILEHEADER) + infoh.biSize, fileh.bfOffBits);
    }

    // -- Read pixmap data into memory

    size_t pixmap_row_bytes = 4 * (((infoh.biBitCount / 8) * infoh.biWidth + 3) / 4);
    size_t padding_bytes = pixmap_row_bytes - (infoh.biWidth * (infoh.biBitCount / 8));
    size_t pixmap_cal_bytes = infoh.biHeight * pixmap_row_bytes;
    size_t pixmap_bytes = infoh.biSizeImage;
    printf("Row size = %zu; padding bytes = %zu\n", pixmap_row_bytes, padding_bytes);
    printf("Pixmap size in bytes: %zu (w*h+padding) = %zu (header)\n", pixmap_cal_bytes,
           pixmap_bytes);

    if (pixmap_bytes != pixmap_cal_bytes) {
        fprintf(stderr, "Bad image size\n");
        fclose(f);
        return 2;
    }

    unsigned char* pixmap_data = malloc(pixmap_bytes);
    fseek(f, fileh.bfOffBits, SEEK_SET);
    if (fread(pixmap_data, pixmap_bytes, 1, f) != 1) {
        perror("Failed to read pixmap data");
        return 2;
    }

    fclose(f);

    // Histogram

    if (argc == 2) {
        HIST* h = histogram_init(pixmap_row_bytes - padding_bytes);  // Usable bytes

        for (int i = 0; i < infoh.biHeight; i++)
            histogram_process_row(h, &pixmap_data[i * pixmap_row_bytes]);

        histogram_finalize(h);
    }

    // Grayscale conversion
    if (argc == 3) {
        FILE* gf = fopen(argv[2], "wb");
        if (gf == NULL) {
            perror("Failed to open output file.");
            return 2;
        }

        // Write grayscale headers (24-bit BGR)

        // size_t oldoffset = fileh.bfOffBits;
        fileh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        infoh.biSize = sizeof(BITMAPINFOHEADER);  // Old version
        fwrite(&fileh, sizeof(BITMAPFILEHEADER), 1, gf);
        fwrite(&infoh, sizeof(BITMAPINFOHEADER), 1, gf);

        // Write grayscale row by row

        static const char padbuf[] = "abcdef";
        for (int i = 0; i < infoh.biHeight; i++) {
            convert_to_grayscale(gf, &pixmap_data[i * pixmap_row_bytes],
                                 pixmap_row_bytes - padding_bytes);
            if (padding_bytes > 0) fwrite(&padbuf[0], padding_bytes, 1, gf);
        }

        fclose(gf);
        fprintf(stderr, "Written grayscale image to %s\n", argv[2]);
    }
    else if (argc < 3) {
        fprintf(stderr, "No output filename for grayscale conversion\n");
    }

    // Steganography decoding
    if (argc == 2) {
        printf("Decode steganography? (Y/n) ");
        char c;
        scanf(" %c", &c);
        if (c == 'Y') main_decode(pixmap_data, pixmap_bytes);
    }

    // Steganography encoding
    if (argc == 4) {
        FILE* gf = fopen(argv[2], "wb");
        if (gf == NULL) {
            perror("Failed to open output file.");
            return 2;
        }

        fprintf(stderr, "Encoding steganography\n");
        uint8_t* enc = main_encode(pixmap_data, pixmap_bytes, argv[3]);

        size_t oldoffset = fileh.bfOffBits;
        fileh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        infoh.biSize = sizeof(BITMAPINFOHEADER);  // Old version
        fwrite(&fileh, sizeof(BITMAPFILEHEADER), 1, gf);
        fwrite(&infoh, sizeof(BITMAPINFOHEADER), 1, gf);
        fileh.bfOffBits = oldoffset;

        fwrite(enc, pixmap_bytes, 1, gf);
        fclose(gf);
        free(enc);
    }

    // Cleanup

    free(pixmap_data);
    return 0;
}

void convert_to_grayscale(FILE* f, unsigned char* row, size_t row_len)
{
    // write row_len bytes, 24-bit BGR
    for (unsigned int i = 0; i < row_len; i += 3) {
        uint8_t b = row[i + 0];
        uint8_t g = row[i + 1];
        uint8_t r = row[i + 2];
        uint16_t avg = b + g + r;
        avg /= 3;
        uint8_t buf[3] = {avg, avg, avg};
        fwrite(buf, 3, 1, f);
    }
}
