#include <cstdio>
bool __fastcall CCreatureAction_TentacleStrikeGetHit_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_TentacleStrikeGetHit_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_008cfaa0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cfaa0_TEST FAIL\n");
    return 1;
}