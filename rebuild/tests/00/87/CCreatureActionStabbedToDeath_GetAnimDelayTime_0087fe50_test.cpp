#include <cstdio>
int __fastcall CCreatureAction_StabbedToDeath_GetAnimDelayTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_StabbedToDeath_GetAnimDelayTime() == 0) { std::printf("AUTO_TINY_0087fe50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0087fe50_TEST FAIL\n");
    return 1;
}