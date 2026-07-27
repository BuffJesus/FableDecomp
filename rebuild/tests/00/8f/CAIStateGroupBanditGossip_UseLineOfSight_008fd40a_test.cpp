#include <cstdio>
bool __fastcall CAIStateGroup_BanditGossip_UseLineOfSight()
{
    return false;
}
int main()
{
    if (CAIStateGroup_BanditGossip_UseLineOfSight() == false) { std::printf("AUTO_TINY_008fd40a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fd40a_TEST FAIL\n");
    return 1;
}