#include <cstdio>
bool __fastcall CAIStateGroup_MinionWatchFires_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_MinionWatchFires_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_008ff071_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ff071_TEST FAIL\n");
    return 1;
}