#include <cstdio>
bool __fastcall CCreatureAction_SheatheItemToInventory_ForDivineWrath_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_SheatheItemToInventory_ForDivineWrath_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_006a07f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a07f0_TEST FAIL\n");
    return 1;
}