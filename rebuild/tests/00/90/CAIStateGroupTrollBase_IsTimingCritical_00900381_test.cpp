#include <cstdio>
bool __fastcall CAIStateGroup_TrollBase_IsTimingCritical()
{
    return true;
}
int main()
{
    if (CAIStateGroup_TrollBase_IsTimingCritical() == true) { std::printf("AUTO_TINY_00900381_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900381_TEST FAIL\n");
    return 1;
}