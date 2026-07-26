#include <cstdio>

struct CSpecialAbilitiesMultiStrikeDef { int dummy; };

unsigned long __fastcall CSpecialAbilitiesMultiStrikeDef_GetSizeofClass(const CSpecialAbilitiesMultiStrikeDef* self)
{
    return 0x9c;
}

int main()
{
    CSpecialAbilitiesMultiStrikeDef obj;
    unsigned long r = CSpecialAbilitiesMultiStrikeDef_GetSizeofClass(&obj);
    if (r == 0x9c) {
        std::printf("CSpecialAbilitiesMultiStrikeDef_004571e8_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}