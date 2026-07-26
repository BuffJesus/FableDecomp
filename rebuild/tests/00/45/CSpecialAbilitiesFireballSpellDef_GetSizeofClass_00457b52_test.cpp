#include <cstdio>

unsigned long __fastcall CSpecialAbilitiesFireballSpellDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x54;
}

int main()
{
    int dummy = 0;
    unsigned long r = CSpecialAbilitiesFireballSpellDef_GetSizeofClass(&dummy);
    if (r == 0x54) {
        std::printf("CSpecialAbilitiesFireballSpellDef_00457b52_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}