#include <cstdio>
int __fastcall CAIStateGroup_BanditGossip_GetTimeToRespond()
{
    return 90;
}
int main()
{
    if (CAIStateGroup_BanditGossip_GetTimeToRespond() == 90) { std::printf("AUTO_TINY_0090e5c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0090e5c0_TEST FAIL\n");
    return 1;
}