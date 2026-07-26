#include <cstdio>
bool __fastcall CCombatAction_KickThingOnGround_IsToPerformActionOnStrike()
{
    return true;
}
int main()
{
    if (CCombatAction_KickThingOnGround_IsToPerformActionOnStrike() == true) { std::printf("AUTO_TINY_00880020_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00880020_TEST FAIL\n");
    return 1;
}