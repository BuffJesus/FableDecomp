#include <cstdio>
bool __fastcall CCreatureAction_ReleaseUnholyPowerSpell_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_ReleaseUnholyPowerSpell_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008bdc80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdc80_TEST FAIL\n");
    return 1;
}