#include <stdio.h>

unsigned long * __fastcall Fill_n_ulong(unsigned long *dest, unsigned int count, unsigned long *val);

int main()
{
    unsigned long buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = 0xAAAAAAAA;
    unsigned long v = 0x12345678;

    // fill first 5, returns one-past-end
    unsigned long *end = Fill_n_ulong(buf, 5, &v);

    int ok = 1;
    if (end != buf + 5) ok = 0;
    for (int i = 0; i < 5; ++i) if (buf[i] != 0x12345678) ok = 0;
    for (int i = 5; i < 8; ++i) if (buf[i] != 0xAAAAAAAA) ok = 0;

    // count 0 = no-op, returns dest unchanged
    unsigned long one = 0x99;
    unsigned long w = 0xDEAD;
    unsigned long *e2 = Fill_n_ulong(&one, 0, &w);
    if (one != 0x99) ok = 0;
    if (e2 != &one) ok = 0;

    if (ok) printf("FILLN_OK\n");
    else printf("FILLN_BAD\n");
    return ok ? 0 : 1;
}