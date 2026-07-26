#include <cstdio>
bool __fastcall CAIStateGroup_MinionSitAtHome_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_MinionSitAtHome_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_008ff36c_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ff36c_TEST FAIL\n");
    return 1;
}