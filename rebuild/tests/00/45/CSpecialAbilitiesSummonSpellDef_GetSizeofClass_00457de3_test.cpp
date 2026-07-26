#include <cstdio>
typedef unsigned long ulong;

ulong __fastcall CSpecialAbilitiesSummonSpellDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x6c;
}

int main()
{
    ulong r = CSpecialAbilitiesSummonSpellDef_GetSizeofClass((void*)0x12345678);
    if (r == 0x6c) {
        std::printf("CSpecialAbilitiesSummonSpellDef_00457de3_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}