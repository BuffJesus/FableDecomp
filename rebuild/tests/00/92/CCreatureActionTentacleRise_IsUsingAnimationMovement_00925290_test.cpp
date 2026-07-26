#include <cstdio>
bool __fastcall CCreatureAction_TentacleRise_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_TentacleRise_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_00925290_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00925290_TEST FAIL\n");
    return 1;
}