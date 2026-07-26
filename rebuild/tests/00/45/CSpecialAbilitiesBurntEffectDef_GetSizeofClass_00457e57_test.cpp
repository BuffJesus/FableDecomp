#include <cstdio>

struct CSpecialAbilitiesBurntEffectDef {};

unsigned long __fastcall CSpecialAbilitiesBurntEffectDef_GetSizeofClass(const CSpecialAbilitiesBurntEffectDef* self)
{
    (void)self;
    return 0x54;
}

int main()
{
    CSpecialAbilitiesBurntEffectDef obj;
    unsigned long r = CSpecialAbilitiesBurntEffectDef_GetSizeofClass(&obj);
    if (r != 0x54) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CSpecialAbilitiesBurntEffectDef_00457e57_TEST PASS\n");
    return 0;
}