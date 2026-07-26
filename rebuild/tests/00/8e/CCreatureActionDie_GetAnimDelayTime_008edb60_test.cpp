#include <cstdio>
int __fastcall CCreatureAction_Die_GetAnimDelayTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_Die_GetAnimDelayTime() == 0) { std::printf("AUTO_TINY_008edb60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008edb60_TEST FAIL\n");
    return 1;
}