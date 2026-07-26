#include <cstdio>
int __fastcall CCreatureAction_DragonFlight_GetAnimDelayTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_DragonFlight_GetAnimDelayTime() == 0) { std::printf("AUTO_TINY_008f55e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f55e0_TEST FAIL\n");
    return 1;
}