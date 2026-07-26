#include <cstdio>

struct CSpecialAbilitiesForcePushDef;

unsigned long __fastcall CSpecialAbilitiesForcePushDef_GetSizeofClass(const CSpecialAbilitiesForcePushDef* self)
{
    (void)self;
    return 0xa4u;
}

int main()
{
    unsigned long r = CSpecialAbilitiesForcePushDef_GetSizeofClass(0);
    if (r == 0xa4u) {
        std::printf("CSpecialAbilitiesForcePushDef_004576b9_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}