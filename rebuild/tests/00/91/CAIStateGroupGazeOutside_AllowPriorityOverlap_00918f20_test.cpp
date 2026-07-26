#include <cstdio>
bool __fastcall CAIStateGroup_GazeOutside_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_GazeOutside_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_00918f20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00918f20_TEST FAIL\n");
    return 1;
}