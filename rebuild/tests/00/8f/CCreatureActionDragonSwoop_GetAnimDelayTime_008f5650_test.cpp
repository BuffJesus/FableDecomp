#include <cstdio>
int __fastcall CCreatureAction_DragonSwoop_GetAnimDelayTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_DragonSwoop_GetAnimDelayTime() == 0) { std::printf("AUTO_TINY_008f5650_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5650_TEST FAIL\n");
    return 1;
}