#include <cstdio>
bool __fastcall CAIStateGroup_MinionSleep_InitCreaturePosition()
{
    return false;
}
int main()
{
    if (CAIStateGroup_MinionSleep_InitCreaturePosition() == false) { std::printf("AUTO_TINY_0092c4d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0092c4d0_TEST FAIL\n");
    return 1;
}