#include <cstdio>
int __fastcall CSBGoal_GetType()
{
    return 1;
}
int main()
{
    if (CSBGoal_GetType() == 1) { std::printf("AUTO_TINY_00a20860_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a20860_TEST FAIL\n");
    return 1;
}