#include <cstdio>
bool __fastcall CCreatureAction_ReleaseUnholyPowerSpell_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleaseUnholyPowerSpell_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008bdc90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdc90_TEST FAIL\n");
    return 1;
}