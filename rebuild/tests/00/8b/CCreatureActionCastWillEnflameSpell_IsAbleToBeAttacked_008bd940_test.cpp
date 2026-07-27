#include <cstdio>
bool __fastcall CCreatureAction_CastWillEnflameSpell_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_CastWillEnflameSpell_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008bd940_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bd940_TEST FAIL\n");
    return 1;
}