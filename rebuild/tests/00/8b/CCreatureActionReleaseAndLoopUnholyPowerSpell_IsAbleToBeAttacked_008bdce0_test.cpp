#include <cstdio>
bool __fastcall CCreatureAction_ReleaseAndLoopUnholyPowerSpell_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleaseAndLoopUnholyPowerSpell_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008bdce0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdce0_TEST FAIL\n");
    return 1;
}