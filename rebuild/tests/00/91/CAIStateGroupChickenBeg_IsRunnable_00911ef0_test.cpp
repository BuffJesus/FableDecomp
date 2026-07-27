#include <cstdio>
bool __fastcall CAIStateGroup_ChickenBeg_IsRunnable()
{
    return false;
}
int main()
{
    if (CAIStateGroup_ChickenBeg_IsRunnable() == false) { std::printf("AUTO_TINY_00911ef0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00911ef0_TEST FAIL\n");
    return 1;
}