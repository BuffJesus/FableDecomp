#include <cstdio>
bool __fastcall CAIStateGroup_WhisperPotionCombat_IsCombatStateGroup()
{
    return false;
}
int main()
{
    if (CAIStateGroup_WhisperPotionCombat_IsCombatStateGroup() == false) { std::printf("AUTO_TINY_009031d7_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009031d7_TEST FAIL\n");
    return 1;
}