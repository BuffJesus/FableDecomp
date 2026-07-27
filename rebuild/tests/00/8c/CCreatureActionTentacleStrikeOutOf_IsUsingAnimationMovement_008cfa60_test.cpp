#include <cstdio>
bool __fastcall CCreatureAction_TentacleStrikeOutOf_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_TentacleStrikeOutOf_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_008cfa60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cfa60_TEST FAIL\n");
    return 1;
}