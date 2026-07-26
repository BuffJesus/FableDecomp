#include <cstdio>
bool __fastcall CAIStateGroup_WanderAroundShops_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_WanderAroundShops_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_009000ef_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009000ef_TEST FAIL\n");
    return 1;
}