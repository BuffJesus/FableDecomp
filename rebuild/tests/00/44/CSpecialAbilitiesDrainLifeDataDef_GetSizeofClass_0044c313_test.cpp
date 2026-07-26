#include <cstdio>

struct CSpecialAbilitiesDrainLifeDataDef { unsigned char pad[0x2c]; };

unsigned long __fastcall CSpecialAbilitiesDrainLifeDataDef_GetSizeofClass(const CSpecialAbilitiesDrainLifeDataDef* self)
{
    (void)self;
    return sizeof(CSpecialAbilitiesDrainLifeDataDef);
}

int main()
{
    CSpecialAbilitiesDrainLifeDataDef obj;
    unsigned long r = CSpecialAbilitiesDrainLifeDataDef_GetSizeofClass(&obj);
    if (r == 0x2c) {
        std::printf("CSpecialAbilitiesDrainLifeDataDef_0044c313_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL: got %lu expected 44\n", r);
    return 1;
}