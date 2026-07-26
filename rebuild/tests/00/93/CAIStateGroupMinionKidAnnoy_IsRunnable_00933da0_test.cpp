#include <cstdio>
bool __fastcall CAIStateGroup_MinionKidAnnoy_IsRunnable()
{
    return false;
}
int main()
{
    if (CAIStateGroup_MinionKidAnnoy_IsRunnable() == false) { std::printf("AUTO_TINY_00933da0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00933da0_TEST FAIL\n");
    return 1;
}