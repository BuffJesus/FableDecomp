#include <cstdio>
int __fastcall CContainerRewardHeroDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CContainerRewardHeroDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004e2497_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e2497_TEST FAIL\n");
    return 1;
}