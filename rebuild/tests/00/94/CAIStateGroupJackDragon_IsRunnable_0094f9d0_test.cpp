#include <cstdio>
bool __fastcall CAIStateGroup_JackDragon_IsRunnable()
{
    return true;
}
int main()
{
    if (CAIStateGroup_JackDragon_IsRunnable() == true) { std::printf("AUTO_TINY_0094f9d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0094f9d0_TEST FAIL\n");
    return 1;
}