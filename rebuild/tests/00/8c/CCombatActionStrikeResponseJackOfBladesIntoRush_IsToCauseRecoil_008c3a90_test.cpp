#include <cstdio>
bool __fastcall CCombatAction_StrikeResponseJackOfBladesIntoRush_IsToCauseRecoil()
{
    return false;
}
int main()
{
    if (CCombatAction_StrikeResponseJackOfBladesIntoRush_IsToCauseRecoil() == false) { std::printf("AUTO_TINY_008c3a90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c3a90_TEST FAIL\n");
    return 1;
}