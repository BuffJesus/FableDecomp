#include <cstdio>

struct CSpecialAbilitiesLightningSpellDef {};

unsigned long __fastcall CSpecialAbilitiesLightningSpellDef_GetSizeofClass(const CSpecialAbilitiesLightningSpellDef* self)
{
    (void)self;
    return 0x128;
}

int main()
{
    CSpecialAbilitiesLightningSpellDef obj;
    unsigned long v = CSpecialAbilitiesLightningSpellDef_GetSizeofClass(&obj);
    if (v == 0x128) {
        std::printf("CSpecialAbilitiesLightningSpellDef_00457c7e_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", v);
    return 1;
}