#include <cstdio>
int __fastcall CCreatureAction_BeDazed_GetAnimDelayTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_BeDazed_GetAnimDelayTime() == 0) { std::printf("AUTO_TINY_00865a90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00865a90_TEST FAIL\n");
    return 1;
}