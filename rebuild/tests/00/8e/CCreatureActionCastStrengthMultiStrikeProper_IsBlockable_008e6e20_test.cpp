#include <cstdio>
bool __fastcall CCreatureAction_CastStrengthMultiStrikeProper_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_CastStrengthMultiStrikeProper_IsBlockable() == false) { std::printf("AUTO_TINY_008e6e20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008e6e20_TEST FAIL\n");
    return 1;
}