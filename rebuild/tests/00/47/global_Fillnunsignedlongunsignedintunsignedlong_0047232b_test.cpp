#include <cstdio>

void __fastcall _Fill_n(unsigned long *_First, unsigned int _Count, const unsigned long &_Val);

int main()
{
    unsigned long buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = 0xDEADBEEFul;

    unsigned long val = 0x11223344ul;
    _Fill_n(buf, 5u, val);

    bool ok = true;
    for (int i = 0; i < 5; ++i) if (buf[i] != 0x11223344ul) ok = false;
    for (int i = 5; i < 8; ++i) if (buf[i] != 0xDEADBEEFul) ok = false;

    // count == 0 : no writes
    unsigned long buf2[3] = { 1ul, 2ul, 3ul };
    unsigned long v2 = 9ul;
    _Fill_n(buf2, 0u, v2);
    if (buf2[0] != 1ul || buf2[1] != 2ul || buf2[2] != 3ul) ok = false;

    // single element
    unsigned long one = 7ul;
    unsigned long vv = 0x55u;
    _Fill_n(&one, 1u, vv);
    if (one != 0x55u) ok = false;

    if (ok) printf("FILLN_OK_47232b\n");
    else    printf("FILLN_FAIL\n");
    return ok ? 0 : 1;
}