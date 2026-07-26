#include <cstdio>
bool __fastcall CAIStateGroup_JackOfBladesBase_IsTimingCritical()
{
    return true;
}
int main()
{
    if (CAIStateGroup_JackOfBladesBase_IsTimingCritical() == true) { std::printf("AUTO_TINY_00903043_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00903043_TEST FAIL\n");
    return 1;
}