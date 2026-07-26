#include <cstdio>
bool __fastcall CAIStateGroup_CreatureAdvanceInCover_IsCombatStateGroup()
{
    return true;
}
int main()
{
    if (CAIStateGroup_CreatureAdvanceInCover_IsCombatStateGroup() == true) { std::printf("AUTO_TINY_00903151_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00903151_TEST FAIL\n");
    return 1;
}