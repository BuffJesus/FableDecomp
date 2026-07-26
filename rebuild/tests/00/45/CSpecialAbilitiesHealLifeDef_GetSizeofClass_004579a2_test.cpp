#include <cstdio>

struct CSpecialAbilitiesHealLifeDef { char pad[0x60]; };

unsigned long __fastcall CSpecialAbilitiesHealLifeDef_GetSizeofClass(const CSpecialAbilitiesHealLifeDef* self)
{
    (void)self;
    return 0x60;
}

int main()
{
    CSpecialAbilitiesHealLifeDef obj;
    unsigned long v = CSpecialAbilitiesHealLifeDef_GetSizeofClass(&obj);
    if (v == 0x60) {
        std::printf("CSpecialAbilitiesHealLifeDef_004579a2_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL: got %lu\n", v);
    return 1;
}