#include <cstdio>
bool __fastcall CAIStateGroup_JackDragon_IsTimingCritical()
{
    return true;
}
int main()
{
    if (CAIStateGroup_JackDragon_IsTimingCritical() == true) { std::printf("AUTO_TINY_009006be_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009006be_TEST FAIL\n");
    return 1;
}