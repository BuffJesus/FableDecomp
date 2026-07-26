#include <cstdio>
bool __fastcall CCreatureAction_ReleasePhysicalShieldSpell_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_ReleasePhysicalShieldSpell_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008bd9c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bd9c0_TEST FAIL\n");
    return 1;
}