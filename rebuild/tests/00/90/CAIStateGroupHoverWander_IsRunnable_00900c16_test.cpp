#include <cstdio>
bool __fastcall CAIStateGroup_HoverWander_IsRunnable()
{
    return true;
}
int main()
{
    if (CAIStateGroup_HoverWander_IsRunnable() == true) { std::printf("AUTO_TINY_00900c16_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900c16_TEST FAIL\n");
    return 1;
}