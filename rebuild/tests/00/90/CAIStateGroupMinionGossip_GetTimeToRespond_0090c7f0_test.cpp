#include <cstdio>
int __fastcall CAIStateGroup_MinionGossip_GetTimeToRespond()
{
    return 60;
}
int main()
{
    if (CAIStateGroup_MinionGossip_GetTimeToRespond() == 60) { std::printf("AUTO_TINY_0090c7f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0090c7f0_TEST FAIL\n");
    return 1;
}