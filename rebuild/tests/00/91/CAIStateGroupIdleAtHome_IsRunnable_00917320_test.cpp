#include <cstdio>
bool __fastcall CAIStateGroup_IdleAtHome_IsRunnable()
{
    return false;
}
int main()
{
    if (CAIStateGroup_IdleAtHome_IsRunnable() == false) { std::printf("AUTO_TINY_00917320_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00917320_TEST FAIL\n");
    return 1;
}