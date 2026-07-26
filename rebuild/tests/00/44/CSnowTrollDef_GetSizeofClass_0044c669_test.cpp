#include <cstdio>

struct CSnowTrollDef { int dummy; };

unsigned long __fastcall CSnowTrollDef_GetSizeofClass(const CSnowTrollDef* self)
{
    (void)self;
    return 0x60;
}

int main()
{
    CSnowTrollDef d;
    unsigned long r = CSnowTrollDef_GetSizeofClass(&d);
    if (r != 0x60) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CSnowTrollDef_0044c669_TEST PASS\n");
    return 0;
}