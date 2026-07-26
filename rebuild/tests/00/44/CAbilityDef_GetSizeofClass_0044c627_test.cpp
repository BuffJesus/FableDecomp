#include <cstdio>

typedef unsigned long ulong;

struct CAbilityDef {};

ulong __fastcall CAbilityDef_GetSizeofClass(const CAbilityDef* self)
{
    (void)self;
    return 0x2c;
}

int main()
{
    CAbilityDef d;
    ulong r = CAbilityDef_GetSizeofClass(&d);
    if (r == 0x2c) {
        std::printf("CAbilityDef_0044c627_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}