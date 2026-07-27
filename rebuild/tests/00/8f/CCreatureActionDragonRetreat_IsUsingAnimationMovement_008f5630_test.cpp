#include <cstdio>
bool __fastcall CCreatureAction_DragonRetreat_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonRetreat_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_008f5630_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5630_TEST FAIL\n");
    return 1;
}