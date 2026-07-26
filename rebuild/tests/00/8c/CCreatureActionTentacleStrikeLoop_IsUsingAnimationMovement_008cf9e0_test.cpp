#include <cstdio>
bool __fastcall CCreatureAction_TentacleStrikeLoop_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_TentacleStrikeLoop_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_008cf9e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cf9e0_TEST FAIL\n");
    return 1;
}