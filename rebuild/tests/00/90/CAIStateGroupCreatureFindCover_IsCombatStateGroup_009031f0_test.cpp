#include <cstdio>
bool __fastcall CAIStateGroup_CreatureFindCover_IsCombatStateGroup()
{
    return true;
}
int main()
{
    if (CAIStateGroup_CreatureFindCover_IsCombatStateGroup() == true) { std::printf("AUTO_TINY_009031f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009031f0_TEST FAIL\n");
    return 1;
}