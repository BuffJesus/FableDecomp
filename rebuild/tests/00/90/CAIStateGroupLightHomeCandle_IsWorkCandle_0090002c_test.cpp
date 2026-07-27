#include <cstdio>
bool __fastcall CAIStateGroup_LightHomeCandle_IsWorkCandle()
{
    return false;
}
int main()
{
    if (CAIStateGroup_LightHomeCandle_IsWorkCandle() == false) { std::printf("AUTO_TINY_0090002c_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0090002c_TEST FAIL\n");
    return 1;
}