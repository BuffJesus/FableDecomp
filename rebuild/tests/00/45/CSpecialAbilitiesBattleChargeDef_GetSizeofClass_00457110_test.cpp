#include <cstdio>

struct CSpecialAbilitiesBattleChargeDef {};

unsigned long __fastcall CSpecialAbilitiesBattleChargeDef_GetSizeofClass(const CSpecialAbilitiesBattleChargeDef* self)
{
    (void)self;
    return 0xa4;
}

int main()
{
    CSpecialAbilitiesBattleChargeDef obj;
    unsigned long r = CSpecialAbilitiesBattleChargeDef_GetSizeofClass(&obj);
    if (r == 0xa4) {
        std::printf("CSpecialAbilitiesBattleChargeDef_00457110_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got=%lu\n", r);
    return 1;
}