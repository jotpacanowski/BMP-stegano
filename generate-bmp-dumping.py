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
  // -- version 4
  DWORD        bV4RedMask;
  DWORD        bV4GreenMask;
  DWORD        bV4BlueMask;
  DWORD        bV4AlphaMask;
  DWORD        bV4CSType;
  CIEXYZTRIPLE bV4Endpoints;
  DWORD        bV4GammaRed;
  DWORD        bV4GammaGreen;
  DWORD        bV4GammaBlue;
  // -- version 5
  DWORD        bV5Intent;
  DWORD        bV5ProfileData;
  DWORD        bV5ProfileSize;
  DWORD        bV5Reserved;
"""


def parse_c_struct(s):
    return [x.strip(' \t;').split() for x in s.splitlines()
            if x.strip() and not x.strip().startswith('//')]


def generate_single_printf(xtype, name):
    if xtype == 'CIEXYZTRIPLE':
        return f'    printf("  {name:>18s} : ???\\n");'

    return (f'    printf("  {name:>18s} : {HEXFORMAT[xtype]}  {DECFORMAT[xtype]}\\n",'
            f' x->{name}, x->{name});')


def generate_func(s, name: str, typename: str):
    s = parse_c_struct(s)
    lines = [f'static inline void dump_{name}({typename}* x){{',
             f'    printf(\"{typename}\\n\");']
    for x in s:
        if x[1] == 'bV4RedMask':  # First v4 field
            lines.append('    if(x->biSize < 108)\n        return;')
        elif x[1] == 'bV5Intent':  # First v5 field
            lines.append('    if(x->biSize < 124)\n        return;')
        lines.append(generate_single_printf(*x))
    lines.append('}')
    return '\n'.join(lines)


def print_c_include():
    print('#pragma once')
    print('#include "bmp_headers.h"')
    print('// $ python ./generate-bmp-dumping.py')
    print(generate_func(BFH_DEF, 'file_header', 'BITMAPFILEHEADER'))
    print('')
    print(generate_func(BIH_DEF, 'info_header', 'BITMAPINFOHEADER'))


print_c_include()

if __name__ == '__main__':
    with open('bmp_dumping.h', 'w', encoding='utf-8') as f:
        with redirect_stdout(f):
            print_c_include()
