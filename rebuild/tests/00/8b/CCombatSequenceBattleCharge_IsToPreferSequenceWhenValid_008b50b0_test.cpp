#include <cstdio>
bool __fastcall CCombatSequence_BattleCharge_IsToPreferSequenceWhenValid()
{
    return true;
}
int main()
{
    if (CCombatSequence_BattleCharge_IsToPreferSequenceWhenValid() == true) { std::printf("AUTO_TINY_008b50b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b50b0_TEST FAIL\n");
    return 1;
}