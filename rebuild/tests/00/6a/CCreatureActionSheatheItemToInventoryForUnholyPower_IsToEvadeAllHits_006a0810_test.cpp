#include <cstdio>
bool __fastcall CCreatureAction_SheatheItemToInventory_ForUnholyPower_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_SheatheItemToInventory_ForUnholyPower_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_006a0810_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a0810_TEST FAIL\n");
    return 1;
}