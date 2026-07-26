#include <cstdio>
bool __fastcall CAIStateGroup_KidWanderNearKids_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_KidWanderNearKids_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_008fd99e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fd99e_TEST FAIL\n");
    return 1;
}