#include <cstdio>
bool __fastcall CAIStateGroup_MazeDefensiveMeleeCombat_IsTimingCritical()
{
    return true;
}
int main()
{
    if (CAIStateGroup_MazeDefensiveMeleeCombat_IsTimingCritical() == true) { std::printf("AUTO_TINY_009007a8_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009007a8_TEST FAIL\n");
    return 1;
}