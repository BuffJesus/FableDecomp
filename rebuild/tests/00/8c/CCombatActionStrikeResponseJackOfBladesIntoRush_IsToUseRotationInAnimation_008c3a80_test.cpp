#include <cstdio>
bool __fastcall CCombatAction_StrikeResponseJackOfBladesIntoRush_IsToUseRotationInAnimation()
{
    return false;
}
int main()
{
    if (CCombatAction_StrikeResponseJackOfBladesIntoRush_IsToUseRotationInAnimation() == false) { std::printf("AUTO_TINY_008c3a80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c3a80_TEST FAIL\n");
    return 1;
}