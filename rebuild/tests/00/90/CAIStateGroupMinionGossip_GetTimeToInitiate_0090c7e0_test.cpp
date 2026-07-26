#include <cstdio>
int __fastcall CAIStateGroup_MinionGossip_GetTimeToInitiate()
{
    return 70;
}
int main()
{
    if (CAIStateGroup_MinionGossip_GetTimeToInitiate() == 70) { std::printf("AUTO_TINY_0090c7e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0090c7e0_TEST FAIL\n");
    return 1;
}