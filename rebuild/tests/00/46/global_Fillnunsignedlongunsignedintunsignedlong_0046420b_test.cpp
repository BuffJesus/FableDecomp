#include <stdio.h>
typedef unsigned long ulong;

extern "C" void __fastcall
Fill_n_(ulong *destination, ulong count, const ulong *value);

int main()
{
    ulong buf[8];
    int i;
    for (i = 0; i < 8; ++i) buf[i] = 0xDEADBEEFul;
    ulong val = 0x12345678ul;
    Fill_n_(buf, 5, &val);
    int ok = 1;
    for (i = 0; i < 5; ++i) if (buf[i] != 0x12345678ul) ok = 0;
    for (i = 5; i < 8; ++i) if (buf[i] != 0xDEADBEEFul) ok = 0;
    ulong val2 = 0x99ul;
    Fill_n_(buf, 0, &val2);
    if (buf[0] != 0x12345678ul) ok = 0;
    printf(ok ? "FILLN_OK\n" : "FILLN_BAD\n");
    return ok ? 0 : 1;
}