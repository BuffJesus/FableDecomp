#include <cstdio>

struct CExplosiveTrailDef { int dummy; };

unsigned long __fastcall CExplosiveTrailDef_GetSizeofClass(const CExplosiveTrailDef* self)
{
    (void)self;
    return 0x4c;
}

int main()
{
    CExplosiveTrailDef obj;
    if (CExplosiveTrailDef_GetSizeofClass(&obj) == 0x4c) {
        std::printf("CExplosiveTrailDef_0044c6ab_TEST PASS\n");
        return 0;
    }
    std::printf("CExplosiveTrailDef_0044c6ab_TEST FAIL\n");
    return 1;
}