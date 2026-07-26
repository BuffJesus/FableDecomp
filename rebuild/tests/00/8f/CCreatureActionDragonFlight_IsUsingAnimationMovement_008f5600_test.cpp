#include <cstdio>
bool __fastcall CCreatureAction_DragonFlight_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonFlight_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_008f5600_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5600_TEST FAIL\n");
    return 1;
}