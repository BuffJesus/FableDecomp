#include <cstdio>
bool __fastcall CAIStateGroup_HoverHornetQueenTakeDamage_IsCombatStateGroup()
{
    return false;
}
int main()
{
    if (CAIStateGroup_HoverHornetQueenTakeDamage_IsCombatStateGroup() == false) { std::printf("AUTO_TINY_00900d75_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900d75_TEST FAIL\n");
    return 1;
}