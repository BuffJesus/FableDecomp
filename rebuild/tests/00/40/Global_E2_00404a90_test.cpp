#include <stdio.h>

unsigned long _DAT_013b7cec;
unsigned long _DAT_013b7cfc;
unsigned long _DAT_013b7ce4;
unsigned long _DAT_013b7ce8;
unsigned char DAT_013b7cf0;
unsigned char DAT_013b7cf1;
unsigned char DAT_013b7cf2;
unsigned long _DAT_013b7cf4;
unsigned long _DAT_013b7cf8;
unsigned long _DAT_013b7d00;
unsigned long _DAT_013b7d04;
unsigned long _DAT_013b7d08;
unsigned long _DAT_013b7d0c;
unsigned long _DAT_013b7d10;
unsigned long _DAT_013b7d14;
unsigned char DAT_013b7d18;
unsigned long _DAT_013b7d1c;
unsigned char DAT_013b7d20;
unsigned char DAT_013b7d21;

void __cdecl $E2();

int main()
{
    $E2();
    if (_DAT_013b7cec != 0xff000000UL || _DAT_013b7cfc != 0xff000000UL) return 1;
    if (_DAT_013b7ce4 != 0 || _DAT_013b7ce8 != 0x00404040UL) return 2;
    if (DAT_013b7cf0 != 0x18 || DAT_013b7cf1 != 0xa0 || DAT_013b7cf2 != 0) return 3;
    if (_DAT_013b7cf4 != 0 || _DAT_013b7cf8 != 0xffffffffUL || _DAT_013b7d00 != 0) return 4;
    if (_DAT_013b7d04 != 0x00ffff00UL || _DAT_013b7d08 != 0xffUL ||
        _DAT_013b7d0c != 0xff00UL) return 5;
    if (_DAT_013b7d10 != 0xff0000UL || _DAT_013b7d14 != 0xff0000UL) return 6;
    if (DAT_013b7d18 != 0x80 || _DAT_013b7d1c != 0 ||
        DAT_013b7d20 != 2 || DAT_013b7d21 != 1) return 7;
    printf("GLOBAL_E2_00404A90_TEST PASS\n");
    return 0;
}
