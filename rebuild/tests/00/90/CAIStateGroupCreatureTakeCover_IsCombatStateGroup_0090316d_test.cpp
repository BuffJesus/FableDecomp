#include <cstdio>
bool __fastcall CAIStateGroup_CreatureTakeCover_IsCombatStateGroup()
{
    return true;
}
int main()
{
    if (CAIStateGroup_CreatureTakeCover_IsCombatStateGroup() == true) { std::printf("AUTO_TINY_0090316d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0090316d_TEST FAIL\n");
    return 1;
}