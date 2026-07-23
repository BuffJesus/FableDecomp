#include <cstdio>

typedef unsigned long ulong;

ulong __fastcall CSpecialAbilitiesAssassinRushDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x8c;
}

int main()
{
    char dummy[16];
    ulong r = CSpecialAbilitiesAssassinRushDef_GetSizeofClass(dummy);
    if (r == 0x8c) {
        std::printf("CSpecialAbilitiesAssassinRushDef_00457095_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}