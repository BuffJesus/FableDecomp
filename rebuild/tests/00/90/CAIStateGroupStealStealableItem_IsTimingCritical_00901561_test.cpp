#include <cstdio>
bool __fastcall CAIStateGroup_StealStealableItem_IsTimingCritical()
{
    return true;
}
int main()
{
    if (CAIStateGroup_StealStealableItem_IsTimingCritical() == true) { std::printf("AUTO_TINY_00901561_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00901561_TEST FAIL\n");
    return 1;
}