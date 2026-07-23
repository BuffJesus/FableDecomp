#include <cstdio>

struct CSpecialAbilitiesEnflameDef { char pad[0xa8]; };

unsigned long __fastcall CSpecialAbilitiesEnflameDef_GetSizeofClass(const CSpecialAbilitiesEnflameDef* self)
{
    (void)self;
    return 0xa8;
}

int main()
{
    if (CSpecialAbilitiesEnflameDef_GetSizeofClass(0) == 0xa8) {
        std::printf("CSpecialAbilitiesEnflameDef_00457738_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}