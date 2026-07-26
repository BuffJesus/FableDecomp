#include <cstdio>
bool __fastcall CAIStateGroup_HoverAttack_IsCombatStateGroup()
{
    return true;
}
int main()
{
    if (CAIStateGroup_HoverAttack_IsCombatStateGroup() == true) { std::printf("AUTO_TINY_00900c4d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900c4d_TEST FAIL\n");
    return 1;
}