#include <cstdio>
bool __fastcall CCreatureAction_DragonStrafeOutOf_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonStrafeOutOf_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_008e4b30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008e4b30_TEST FAIL\n");
    return 1;
}