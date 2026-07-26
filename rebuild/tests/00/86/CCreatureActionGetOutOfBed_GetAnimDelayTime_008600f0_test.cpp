#include <cstdio>
int __fastcall CCreatureAction_GetOutOfBed_GetAnimDelayTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_GetOutOfBed_GetAnimDelayTime() == 0) { std::printf("AUTO_TINY_008600f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008600f0_TEST FAIL\n");
    return 1;
}