#include <cstdio>
int __fastcall CCreatureAction_DragonRetreat_GetAnimDelayTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_DragonRetreat_GetAnimDelayTime() == 0) { std::printf("AUTO_TINY_008f5620_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5620_TEST FAIL\n");
    return 1;
}