#include <cstdio>
bool __fastcall CCreatureAction_RemovePhysicalShieldSpell_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_RemovePhysicalShieldSpell_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008bd9e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bd9e0_TEST FAIL\n");
    return 1;
}