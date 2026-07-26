#include <cstdio>
bool __fastcall CAIStateGroup_CarryCrateAndFollow_IsTimingCritical()
{
    return true;
}
int main()
{
    if (CAIStateGroup_CarryCrateAndFollow_IsTimingCritical() == true) { std::printf("AUTO_TINY_00901697_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00901697_TEST FAIL\n");
    return 1;
}