#include <cstdio>

struct CSpecialEffectsDef { int dummy; };

unsigned long __fastcall CSpecialEffectsDef_GetSizeofClass(CSpecialEffectsDef* self)
{
    (void)self;
    return 0x34;
}

int main()
{
    CSpecialEffectsDef d;
    unsigned long r = CSpecialEffectsDef_GetSizeofClass(&d);
    if (r == 0x34) {
        std::printf("CSpecialEffectsDef_00454643_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}