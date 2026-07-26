#include <cstdio>
bool __fastcall CCombatAction_KickThingOnGround_IsToHitProneThings()
{
    return true;
}
int main()
{
    if (CCombatAction_KickThingOnGround_IsToHitProneThings() == true) { std::printf("AUTO_TINY_00880030_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00880030_TEST FAIL\n");
    return 1;
}