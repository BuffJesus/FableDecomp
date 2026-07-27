#include <cstdio>
bool __fastcall CCreatureAction_PlayCombatAnimationWithLooking_IsPreventingLookTurning()
{
    return false;
}
int main()
{
    if (CCreatureAction_PlayCombatAnimationWithLooking_IsPreventingLookTurning() == false) { std::printf("AUTO_TINY_008b8690_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b8690_TEST FAIL\n");
    return 1;
}