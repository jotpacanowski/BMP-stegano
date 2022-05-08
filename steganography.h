#pragma once
#include <stddef.h>
#include <stdint.h>

uint8_t* main_encode(const uint8_t* pixmap_data, size_t pixmap_bytes, const char* text);
void main_decode(const uint8_t* pixmap_data, size_t pixmap_bytes);
