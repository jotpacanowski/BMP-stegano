#!/usr/bin/python3
# 2022 Jędrzej Pacanowski

from contextlib import redirect_stdout

DECFORMAT = dict(WORD='%hu', DWORD='%u', LONG='%d')
HEXFORMAT = dict(WORD='%#6hx', DWORD='%#10x', LONG='%#10x')

BFH_DEF = """
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
"""

BIH_DEF = """
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
"""


def parse_c_struct(s):
    return [x.strip(' \t;').split() for x in s.splitlines() if x.strip()]


def generate_printfs(arr):
    return ((f'    printf("  {x[1]:>18s} : {HEXFORMAT[x[0]]}  {DECFORMAT[x[0]]}\\n",'
             f' x->{x[1]}, x->{x[1]});')
            for x in arr)


def generate_func(s, name: str, typename: str):
    s = parse_c_struct(s)
    lines = [f'static inline void dump_{name}({typename}* x){{',
             f'    printf(\"{typename}\\n\");']
    lines.extend(generate_printfs(s))
    lines.append('}\n')
    return '\n'.join(lines)


def print_c_include():
    print('#pragma once')
    print('#include "bmp_headers.h"')
    print('// $ python ./generate-bmp-dumping.py > bmp_dumping.h')
    print(generate_func(BFH_DEF, 'file_header', 'BITMAPFILEHEADER'))
    print('')
    print(generate_func(BIH_DEF, 'info_header', 'BITMAPINFOHEADER'))


print_c_include()

if __name__ == '__main__':
    with open('bmp_dumping.h', 'w', encoding='utf-8') as f:
        with redirect_stdout(f):
            print_c_include()
