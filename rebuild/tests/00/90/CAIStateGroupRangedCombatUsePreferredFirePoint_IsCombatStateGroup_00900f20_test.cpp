#include <cstdio>
bool __fastcall CAIStateGroup_RangedCombatUsePreferredFirePoint_IsCombatStateGroup()
{
    return true;
}
int main()
{
    if (CAIStateGroup_RangedCombatUsePreferredFirePoint_IsCombatStateGroup() == true) { std::printf("AUTO_TINY_00900f20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900f20_TEST FAIL\n");
    return 1;
}