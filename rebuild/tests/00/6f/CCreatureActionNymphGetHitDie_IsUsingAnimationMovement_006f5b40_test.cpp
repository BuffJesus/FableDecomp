#include <cstdio>
bool __fastcall CCreatureAction_NymphGetHitDie_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_NymphGetHitDie_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_006f5b40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5b40_TEST FAIL\n");
    return 1;
}