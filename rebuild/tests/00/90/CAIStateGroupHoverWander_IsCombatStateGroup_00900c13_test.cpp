#include <cstdio>
bool __fastcall CAIStateGroup_HoverWander_IsCombatStateGroup()
{
    return false;
}
int main()
{
    if (CAIStateGroup_HoverWander_IsCombatStateGroup() == false) { std::printf("AUTO_TINY_00900c13_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900c13_TEST FAIL\n");
    return 1;
}