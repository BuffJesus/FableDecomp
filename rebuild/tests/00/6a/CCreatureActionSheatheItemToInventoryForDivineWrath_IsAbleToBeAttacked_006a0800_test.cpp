#include <cstdio>
bool __fastcall CCreatureAction_SheatheItemToInventory_ForDivineWrath_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_SheatheItemToInventory_ForDivineWrath_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_006a0800_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a0800_TEST FAIL\n");
    return 1;
}