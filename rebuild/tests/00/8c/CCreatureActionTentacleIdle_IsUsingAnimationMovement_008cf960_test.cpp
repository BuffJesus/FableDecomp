#include <cstdio>
bool __fastcall CCreatureAction_TentacleIdle_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_TentacleIdle_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_008cf960_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cf960_TEST FAIL\n");
    return 1;
}