#include <cstdio>

unsigned long __fastcall CSpecialAbilitiesTurncoatSpellDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x78;
}

int main()
{
    unsigned long r = CSpecialAbilitiesTurncoatSpellDef_GetSizeofClass(0);
    if (r == 0x78) {
        std::printf("CSpecialAbilitiesTurncoatSpellDef_00457ac4_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}