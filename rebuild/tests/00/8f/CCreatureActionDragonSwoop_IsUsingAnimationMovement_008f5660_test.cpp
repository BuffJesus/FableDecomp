#include <cstdio>
bool __fastcall CCreatureAction_DragonSwoop_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonSwoop_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_008f5660_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5660_TEST FAIL\n");
    return 1;
}