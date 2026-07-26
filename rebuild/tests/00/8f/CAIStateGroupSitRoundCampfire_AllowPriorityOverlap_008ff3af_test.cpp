#include <cstdio>
bool __fastcall CAIStateGroup_SitRoundCampfire_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_SitRoundCampfire_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_008ff3af_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ff3af_TEST FAIL\n");
    return 1;
}