#include <cstdio>
bool __fastcall CAIStateGroup_MinionLeaveRegion_IsRunnable()
{
    return false;
}
int main()
{
    if (CAIStateGroup_MinionLeaveRegion_IsRunnable() == false) { std::printf("AUTO_TINY_0091e4a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0091e4a0_TEST FAIL\n");
    return 1;
}