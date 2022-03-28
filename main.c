// 2022 Jędrzej Pacanowski
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp_headers.h"
#include "bmp_dumping.h"

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
	if(argc != 2){
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

	// --

	int sz = bmp_data_after_headers(&infoh);
	void*xd = malloc(sz);
	fread(xd, sz, 1, f);
	puts(xd);  // BGRs is "sRGB" big-endian

	fclose(f);
	return 0;
}
