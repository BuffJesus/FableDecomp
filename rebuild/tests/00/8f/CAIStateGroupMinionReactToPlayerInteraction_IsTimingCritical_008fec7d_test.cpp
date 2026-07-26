#include <cstdio>
bool __fastcall CAIStateGroup_MinionReactToPlayerInteraction_IsTimingCritical()
{
    return true;
}
int main()
{
    if (CAIStateGroup_MinionReactToPlayerInteraction_IsTimingCritical() == true) { std::printf("AUTO_TINY_008fec7d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fec7d_TEST FAIL\n");
    return 1;
}