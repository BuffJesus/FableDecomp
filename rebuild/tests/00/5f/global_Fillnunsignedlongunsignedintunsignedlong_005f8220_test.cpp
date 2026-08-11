#include <stdio.h>

unsigned long * __fastcall Fill_n_impl(unsigned long *dest, unsigned int count, unsigned long *val);

int main()
{
    unsigned long buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = 0;
    unsigned long v = 0xABCD1234;

    unsigned long *end = Fill_n_impl(buf, 5, &v);

    int ok = 1;
    if (end != buf + 5) ok = 0;
    for (int i = 0; i < 5; ++i) if (buf[i] != 0xABCD1234) ok = 0;
    for (int i = 5; i < 8; ++i) if (buf[i] != 0) ok = 0;

    unsigned long v2 = 0x11111111;
    unsigned long *end0 = Fill_n_impl(buf, 0, &v2);
    if (end0 != buf) ok = 0;
    if (buf[0] != 0xABCD1234) ok = 0;

    if (ok) printf("FILLN_OK\n");
    else printf("FILLN_FAIL\n");
    return ok ? 0 : 1;
}