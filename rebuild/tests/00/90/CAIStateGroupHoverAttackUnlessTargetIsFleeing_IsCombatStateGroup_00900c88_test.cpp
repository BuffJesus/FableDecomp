#include <cstdio>
bool __fastcall CAIStateGroup_HoverAttackUnlessTargetIsFleeing_IsCombatStateGroup()
{
    return true;
}
int main()
{
    if (CAIStateGroup_HoverAttackUnlessTargetIsFleeing_IsCombatStateGroup() == true) { std::printf("AUTO_TINY_00900c88_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900c88_TEST FAIL\n");
    return 1;
}