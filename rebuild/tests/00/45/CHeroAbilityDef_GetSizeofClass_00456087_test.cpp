#include <cstdio>

struct CHeroAbilityDef {};

unsigned long __fastcall CHeroAbilityDef_GetSizeofClass(const CHeroAbilityDef* self)
{
    return 0x9c;
}

int main()
{
    CHeroAbilityDef d;
    unsigned long r = CHeroAbilityDef_GetSizeofClass(&d);
    if (r == 0x9c) {
        std::printf("CHeroAbilityDef_00456087_TEST PASS\n");
        return 0;
    }
    std::printf("CHeroAbilityDef_00456087_TEST FAIL got %lu\n", r);
    return 1;
}