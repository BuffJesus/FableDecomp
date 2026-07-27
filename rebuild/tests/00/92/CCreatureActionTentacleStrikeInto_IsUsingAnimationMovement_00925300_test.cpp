#include <cstdio>
bool __fastcall CCreatureAction_TentacleStrikeInto_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_TentacleStrikeInto_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_00925300_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00925300_TEST FAIL\n");
    return 1;
}