#include <cstdio>
bool __fastcall CAIStateGroup_GazeAtHome_AllowPriorityOverlap()
{
    return true;
}
int main()
{
    if (CAIStateGroup_GazeAtHome_AllowPriorityOverlap() == true) { std::printf("AUTO_TINY_00918f50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00918f50_TEST FAIL\n");
    return 1;
}