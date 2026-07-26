#include <cstdio>

struct CSpecialAbilitiesMultiArrowDef {};

unsigned long __fastcall CSpecialAbilitiesMultiArrowDef_GetSizeofClass(const CSpecialAbilitiesMultiArrowDef* self)
{
    (void)self;
    return 0x80;
}

int main()
{
    CSpecialAbilitiesMultiArrowDef obj;
    unsigned long r = CSpecialAbilitiesMultiArrowDef_GetSizeofClass(&obj);
    if (r == 0x80) {
        std::printf("CSpecialAbilitiesMultiArrowDef_00457285_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}