// 2022 Jędrzej Pacanowski

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct HIST {
	size_t row_data_bytes;
	uint64_t *h_red;
	uint64_t *h_green;
	uint64_t *h_blue;
	uint64_t sum_r, sum_g, sum_b;
};

#define NO_HIDE_HIST
#include "histogram.h"

static const uint32_t HISTOGRAM_BINS = 16;


struct HIST* histogram_init(size_t bytes_per_row){
	struct HIST* histogram_object = malloc(sizeof(struct HIST));
	histogram_object->row_data_bytes = bytes_per_row;
	histogram_object->h_red = calloc(HISTOGRAM_BINS, sizeof(uint64_t));
	histogram_object->h_green = calloc(HISTOGRAM_BINS, sizeof(uint64_t));
	histogram_object->h_blue = calloc(HISTOGRAM_BINS, sizeof(uint64_t));
	histogram_object->sum_r = 0;
	histogram_object->sum_g = 0;
	histogram_object->sum_b = 0;
	return histogram_object;
}

void histogram_process_row(struct HIST* const state, const unsigned char* raw_row){
	const size_t bytes = state->row_data_bytes;
	int rgb = 0;
	for(unsigned int i=0; i<bytes; i++, rgb = (rgb+1) % 3){
		int v = raw_row[i] / HISTOGRAM_BINS;
		switch(rgb){
			case 0:  // BGR (!)
				state->h_blue[v] += 1;
				state->sum_b += 1;
				break;
			case 1:
				state->h_green[v] += 1;
				state->sum_g += 1;
				break;
			case 2:
				state->h_red[v] += 1;
				state->sum_r += 1;
				break;
		}
	}
	if(rgb != 0){
		puts("[err] Not all RGB bytes processed (!)");
	}
}

#ifndef HISTOGRAM_BAR_WIDTH
#define HISTOGRAM_BAR_WIDTH 80 // 60 characters long
#endif

static void histogram_print_bar(float value){
	printf(" ");
	int w = value * HISTOGRAM_BAR_WIDTH / 1.0;
	if(w == 0 && value > 0){
		printf("|\n");
		return;
	}

	for(int i=0; i<w; i++)
		printf("#");
	printf("\n");
}

static void histogram_print(const char* color, uint64_t* data, uint64_t max_sum){
	printf("%s\n", color);
	// printf("Sum is %llu\n", sum);
	const int step = 256 / HISTOGRAM_BINS;
	for(unsigned int i=0; i<HISTOGRAM_BINS; i++){
		// double value = data[i];
		// value /= sum;
		double value = 1.f * (double)data[i] / (double)max_sum;
		//   -- %llu  , data[i]
		printf("   %3u-%-3u: %5.2f %% ", i*step, (i+1)*step - 1, 100*value);
		histogram_print_bar(value);
	}
	puts("");
}

void histogram_finalize(struct HIST* state){
	puts("");

	double max_value = state->sum_r;
	if(state->sum_g > max_value) max_value = state->sum_g;
	if(state->sum_b > max_value) max_value = state->sum_b;
	printf(" max_sum = %lf \n", max_value);

	histogram_print("\x1b[31;1mRed\x1b[0m", state->h_red, max_value);
	histogram_print("\x1b[32;1mGreen\x1b[0m", state->h_green, max_value);
	histogram_print("\x1b[34;1mBlue\x1b[0m", state->h_blue, max_value);

	free(state->h_red);
	free(state->h_green);
	free(state->h_blue);
	free(state);
}
