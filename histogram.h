// 2022 Jędrzej Pacanowski
#pragma once

#include <stdlib.h>

#ifdef NO_HIDE_HIST
#define HISTP struct HIST*
#else
typedef void HIST;
#define HISTP HIST*
#endif

HISTP histogram_init(size_t bytes_per_row);
void histogram_process_row(HISTP const, const unsigned char*);
void histogram_finalize(HISTP);
