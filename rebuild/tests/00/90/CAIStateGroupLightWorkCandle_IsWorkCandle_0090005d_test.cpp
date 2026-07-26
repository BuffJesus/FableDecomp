#include <cstdio>
bool __fastcall CAIStateGroup_LightWorkCandle_IsWorkCandle()
{
    return true;
}
int main()
{
    if (CAIStateGroup_LightWorkCandle_IsWorkCandle() == true) { std::printf("AUTO_TINY_0090005d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0090005d_TEST FAIL\n");
    return 1;
}