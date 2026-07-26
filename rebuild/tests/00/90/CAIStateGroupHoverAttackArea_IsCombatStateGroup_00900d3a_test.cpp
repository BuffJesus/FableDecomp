#include <cstdio>
bool __fastcall CAIStateGroup_HoverAttackArea_IsCombatStateGroup()
{
    return true;
}
int main()
{
    if (CAIStateGroup_HoverAttackArea_IsCombatStateGroup() == true) { std::printf("AUTO_TINY_00900d3a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900d3a_TEST FAIL\n");
    return 1;
}