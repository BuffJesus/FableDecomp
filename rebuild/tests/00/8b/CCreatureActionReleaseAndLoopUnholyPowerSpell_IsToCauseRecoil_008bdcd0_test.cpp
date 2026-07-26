#include <cstdio>
bool __fastcall CCreatureAction_ReleaseAndLoopUnholyPowerSpell_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_ReleaseAndLoopUnholyPowerSpell_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008bdcd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdcd0_TEST FAIL\n");
    return 1;
}