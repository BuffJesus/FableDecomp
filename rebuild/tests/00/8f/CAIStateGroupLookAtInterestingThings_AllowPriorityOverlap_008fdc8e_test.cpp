#include <cstdio>
bool __fastcall CAIStateGroup_LookAtInterestingThings_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_LookAtInterestingThings_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_008fdc8e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fdc8e_TEST FAIL\n");
    return 1;
}