#include <cstdio>

struct CSpecialAbilitiesDivineWrathDef { char pad[4]; };

unsigned long __fastcall CSpecialAbilitiesDivineWrathDef_GetSizeofClass(const CSpecialAbilitiesDivineWrathDef* self)
{
    (void)self;
    return 0xfc;
}

int main()
{
    CSpecialAbilitiesDivineWrathDef obj;
    unsigned long r = CSpecialAbilitiesDivineWrathDef_GetSizeofClass(&obj);
    if (r != 0xfc) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CSpecialAbilitiesDivineWrathDef_004580ef_TEST PASS\n");
    return 0;
}