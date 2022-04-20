#include "steganography.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t decode_byte_to_bits(uint8_t byte)
{
	uint64_t result = 0;
	int j = 0;
	for(int i=0; i<8; i++, j += 8){
		uint64_t x = byte & (1 << i);
		result |= (((x!=0)? (uint64_t)1 : 0) << (uint64_t)j);
	}
	return result;
}

uint8_t decode_bits(uint64_t buf)
{
	uint8_t result = 0;
	int j = 0;
	for(int i=0; i<8; i++, j += 8){
		int x = (buf >> j) & 1;
		result |= (x << i);
	}
	return result;
}


uint8_t* main_encode(const uint8_t* pixmap_data, size_t pixmap_bytes, const char* text)
{
	uint8_t* enc = malloc(pixmap_bytes);
	memcpy(enc, pixmap_data, pixmap_bytes);
	uint64_t *p = (uint64_t*)enc;

	if(enc==NULL){
		printf("Failed to malloc()\n");
		exit(2);
	}

	const char *sptr = text;
	const size_t text_len = strlen(text);

	// TODO encode text length

	for(int i=0; i < pixmap_bytes && i <= text_len; i++){ // incl. NUL byte
		if(pixmap_bytes - i < 8)
			break;

		uint64_t buf = *p;
		// printf("Before: %llx \t", buf);
		uint64_t mask = decode_byte_to_bits(*sptr);
		// printf(" %016llx (byte %hhu)\n", mask, *sptr);
		sptr++;

		buf &= 0xfefefefefefefefe;
		buf |= mask;

		// printf("After : %llx \n", buf);
		*p = buf;
		p++;
	}
	return enc;
}

void main_decode(const uint8_t* pixmap_data, size_t pixmap_bytes)
{
	uint8_t *decoded_buf = calloc(pixmap_bytes / 8, 1);
	uint8_t *decoded = decoded_buf;

	for(int i=0; i < pixmap_bytes; i += 8){
		if(pixmap_bytes - i < 8)
			break;
		uint64_t buf;
		memcpy(&buf, &pixmap_data[i], 8);
		uint8_t byte = decode_bits(buf);

		*decoded = byte;
		decoded++;

		if(!isalnum(byte) && byte != ' ')
			break;

		printf("%c", byte);
		// printf("%hhu ", byte);
	}
	printf("\n");
	printf("%s\n", decoded_buf); // May print out garbage
}
