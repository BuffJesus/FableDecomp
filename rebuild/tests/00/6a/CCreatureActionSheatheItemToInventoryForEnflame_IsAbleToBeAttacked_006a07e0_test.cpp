#include <cstdio>
bool __fastcall CCreatureAction_SheatheItemToInventory_ForEnflame_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_SheatheItemToInventory_ForEnflame_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_006a07e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a07e0_TEST FAIL\n");
    return 1;
}