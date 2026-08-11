#include <stdio.h>

void __fastcall Fill_n_unsigned_long(unsigned long *_First, unsigned int _Count, unsigned long &_Val);

int main()
{
    unsigned long buf[5] = {0,0,0,0,0};
    unsigned long val = 0xABCD1234UL;
    Fill_n_unsigned_long(buf, 3, val);
    int ok = (buf[0]==0xABCD1234UL && buf[1]==0xABCD1234UL && buf[2]==0xABCD1234UL && buf[3]==0 && buf[4]==0);

    // zero count leaves buffer untouched
    unsigned long buf2[2] = {1,2};
    Fill_n_unsigned_long(buf2, 0, val);
    ok = ok && (buf2[0]==1 && buf2[1]==2);

    // full fill
    unsigned long buf3[4] = {9,9,9,9};
    unsigned long v2 = 7UL;
    Fill_n_unsigned_long(buf3, 4, v2);
    ok = ok && (buf3[0]==7 && buf3[1]==7 && buf3[2]==7 && buf3[3]==7);

    if (ok) printf("FILLN_OK\n");
    else printf("FILLN_FAIL\n");
    return ok ? 0 : 1;
}