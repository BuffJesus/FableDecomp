#include <cstdio>

struct CSpecialAbilitiesDrainLifeDef { char pad[0x70]; };

unsigned long __fastcall CSpecialAbilitiesDrainLifeDef_GetSizeofClass(const CSpecialAbilitiesDrainLifeDef* self)
{
    (void)self;
    return 0x70;
}

int main()
{
    CSpecialAbilitiesDrainLifeDef obj;
    unsigned long r = CSpecialAbilitiesDrainLifeDef_GetSizeofClass(&obj);
    if (r != 0x70) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CSpecialAbilitiesDrainLifeDef_00457916_TEST PASS\n");
    return 0;
}