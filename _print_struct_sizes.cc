#define _WINDOWS_LEAN_AND_MEAN 1
#include <stdio.h>
#include <windows.h>

#define TS(x, y) printf("sizeof " #x " is %zu = " #y "\n", sizeof(x));
#define PS(x) printf("sizeof " #x " = %zu\n", sizeof(x));
int main()
{
    TS(WORD, 2)
    TS(DWORD, 4)
    TS(LONG, 4)
    puts("");
    TS(FXPT2DOT30, 4)
    TS(CIEXYZ, 3 * 4)
    TS(CIEXYZTRIPLE, 3 * 3 * 4)
    puts("");
    PS(BITMAPFILEHEADER);
    PS(BITMAPINFOHEADER);
    PS(BITMAPV4HEADER);
    PS(BITMAPV5HEADER);
    return 0;
}
