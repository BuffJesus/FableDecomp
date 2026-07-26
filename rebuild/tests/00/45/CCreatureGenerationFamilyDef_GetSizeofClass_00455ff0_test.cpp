#include <cstdio>

struct CCreatureGenerationFamilyDef {};

unsigned long __fastcall CCreatureGenerationFamilyDef_GetSizeofClass(const CCreatureGenerationFamilyDef* self)
{
    (void)self;
    return 0x5c;
}

int main()
{
    CCreatureGenerationFamilyDef obj;
    unsigned long r = CCreatureGenerationFamilyDef_GetSizeofClass(&obj);
    if (r != 0x5c) { std::printf("FAIL: got %lu\n", r); return 1; }
    std::printf("CCreatureGenerationFamilyDef_00455ff0_TEST PASS\n");
    return 0;
}