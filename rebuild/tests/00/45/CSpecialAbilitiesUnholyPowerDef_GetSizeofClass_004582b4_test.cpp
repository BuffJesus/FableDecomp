#include <cstdio>

struct CSpecialAbilitiesUnholyPowerDef;

unsigned long __fastcall CSpecialAbilitiesUnholyPowerDef_GetSizeofClass(const CSpecialAbilitiesUnholyPowerDef* self)
{
    (void)self;
    return 0xfc;
}

int main()
{
    unsigned long r = CSpecialAbilitiesUnholyPowerDef_GetSizeofClass(0);
    if (r != 0xfc) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CSpecialAbilitiesUnholyPowerDef_004582b4_TEST PASS\n");
    return 0;
}