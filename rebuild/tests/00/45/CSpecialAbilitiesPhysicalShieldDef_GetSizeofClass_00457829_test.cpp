#include <cstdio>

struct CSpecialAbilitiesPhysicalShieldDef {};

unsigned long __fastcall CSpecialAbilitiesPhysicalShieldDef_GetSizeofClass(const CSpecialAbilitiesPhysicalShieldDef* self)
{
    (void)self;
    return 0x148u;
}

int main()
{
    CSpecialAbilitiesPhysicalShieldDef obj;
    unsigned long r = CSpecialAbilitiesPhysicalShieldDef_GetSizeofClass(&obj);
    if (r != 0x148u) {
        std::printf("FAIL got %lu\n", r);
        return 1;
    }
    std::printf("CSpecialAbilitiesPhysicalShieldDef_00457829_TEST PASS\n");
    return 0;
}