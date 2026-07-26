#include <cstdio>
bool __fastcall CCreatureAction_PlayHopscotch_IsUsingAnimationMovement()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayHopscotch_IsUsingAnimationMovement() == true) { std::printf("AUTO_TINY_00941bb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00941bb0_TEST FAIL\n");
    return 1;
}