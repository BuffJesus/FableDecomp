#include <cstdio>
bool __fastcall CAIStateGroup_MinionSitOutside_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_MinionSitOutside_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_009021da_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009021da_TEST FAIL\n");
    return 1;
}