#include <cstdio>
bool __fastcall CCreatureAction_SheatheItemToInventory_ForUnholyPower_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_SheatheItemToInventory_ForUnholyPower_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_006a0820_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a0820_TEST FAIL\n");
    return 1;
}