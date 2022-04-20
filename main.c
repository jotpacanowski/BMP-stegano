// 2022 Jędrzej Pacanowski
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp_headers.h"
#include "bmp_dumping.h"
#include "histogram.h"

BITMAPINFOHEADER infoh;
BITMAPFILEHEADER fileh;

void verify_fileh(BITMAPFILEHEADER *x){
	if(x->bfType != 0x424d) // byte-swapped "BM"
		puts("File magic is OK (BM)");
	else
		puts("Wrong file magic");

	if(x->bfReserved1 != 0 || x->bfReserved2 != 0)
		puts("Reserved fields are not zero!");

	if(x->bfOffBits == 54)
		puts("OffsetBits is OK");
	else
		printf("pixmap offset: %u bytes may be hidden\n"
			" (file off %#x to %#x)\n",
			x->bfOffBits - 14 - 40, 54, x->bfOffBits);

	printf("btw, both headers take %u bytes.\n",
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
}

uint32_t bmp_data_after_headers(BITMAPFILEHEADER *x){
	return x->bfOffBits - 14 - 40;
}

void verify_infoh(BITMAPINFOHEADER *x){
	if(x->biSize != sizeof(BITMAPINFOHEADER))
		puts("biSize is meh (not 40) bytes");
	else	puts("biSize is OK");

	if(x->biPlanes != 1)
		puts("biPlanes is not 1");
}

int main(int argc, char** argv){
	if(argc < 2){
		puts("wrong/no filename");
		return 2;
	}
	// TODO: read to memory and then struct pointers

	FILE* f = fopen(argv[1], "rb");
	if(f == NULL){
		perror("Failed to open file");
		return 1;
	}

	if(fread(&fileh, sizeof(BITMAPFILEHEADER), 1, f) != 1){
		perror("Failed to read FILE header");
		fclose(f);
		return 1;
	}

	dump_file_header(&fileh);
	puts("");
	// TODO: verify size of file

	// fileh.bfOffBits; -- offset of the pixmap

	if(fread(&infoh, sizeof(BITMAPINFOHEADER), 1, f) != 1){
		perror("Failed to read INFO header");
		fclose(f);
		return 1;
	}

	dump_info_header(&infoh);

	puts(" - - - ");

	verify_fileh(&fileh);
	verify_infoh(&infoh);

	if(infoh.biCompression != 0){
		printf("Unsupported compression algorithm (%u)\n", infoh.biCompression);
		fclose(f);
		return 1;
	}
	if(infoh.biBitCount != 24){
		printf("Histogram calculation is unsupported (bitCount=%u)\n", infoh.biBitCount);
		fclose(f);
		return 1;
	}

	// --

	if(fileh.bfOffBits != fileh.bfSize + infoh.biSize){
		printf("Possible hidden data after headers (size is %zu, offset is %zu)\n",
			fileh.bfSize + infoh.biSize, fileh.bfOffBits);
	}

	// -- Read pixmap data into memory

	size_t pixmap_row_bytes = 4 * ( ((infoh.biBitCount/8) * infoh.biWidth + 3)/4 );
	size_t padding_bytes = pixmap_row_bytes - (infoh.biWidth * (infoh.biBitCount/8));
	size_t pixmap_cal_bytes = infoh.biHeight * pixmap_row_bytes;
	size_t pixmap_bytes = infoh.biSizeImage;
	printf("Row size = %zu; padding bytes = %zu\n", pixmap_row_bytes, padding_bytes);
	printf("Pixmap size in bytes: %zu (w*h+padding) = %zu (header)\n",
		pixmap_cal_bytes, pixmap_bytes);

	if(pixmap_bytes != pixmap_cal_bytes){
		puts("???");
		fclose(f);
		return 2;
	}

	unsigned char* pixmap_data = malloc(pixmap_bytes);
	fseek(f, fileh.bfOffBits, SEEK_SET);
	if(fread(pixmap_data, pixmap_bytes, 1, f) != 1){
		perror("Failed to read pixmap data");
		return 2;
	}

	fclose(f);
	perror("OK");

	// Histogram

	HIST* h = histogram_init(pixmap_row_bytes - padding_bytes); // Usable bytes
	for(int i=0; i<infoh.biHeight; i++)
		histogram_process_row(h, &pixmap_data[ i * pixmap_row_bytes ]);

	histogram_finalize(h);

	// Cleanup

	free(pixmap_data);
	return 0;
}
