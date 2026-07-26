#include <cstdio>

struct CSpecialAbilitiesElectrocutedEffectDef { char pad[0x6c]; };

unsigned long __fastcall CSpecialAbilitiesElectrocutedEffectDef_GetSizeofClass(const CSpecialAbilitiesElectrocutedEffectDef* self)
{
    (void)self;
    return 0x6c;
}

int main()
{
    CSpecialAbilitiesElectrocutedEffectDef obj;
    unsigned long r = CSpecialAbilitiesElectrocutedEffectDef_GetSizeofClass(&obj);
    if (r == 0x6c) {
        std::printf("CSpecialAbilitiesElectrocutedEffectDef_00457eb6_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}