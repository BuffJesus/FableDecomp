#include <cstdio>
bool __fastcall CCreatureAction_ReleaseWillEnflameSpell_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleaseWillEnflameSpell_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008bd980_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bd980_TEST FAIL\n");
    return 1;
}