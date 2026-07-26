#include <cstdio>
bool __fastcall CAIStateGroup_HoverHornetQueen_IsCombatStateGroup()
{
    return false;
}
int main()
{
    if (CAIStateGroup_HoverHornetQueen_IsCombatStateGroup() == false) { std::printf("AUTO_TINY_00903134_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00903134_TEST FAIL\n");
    return 1;
}