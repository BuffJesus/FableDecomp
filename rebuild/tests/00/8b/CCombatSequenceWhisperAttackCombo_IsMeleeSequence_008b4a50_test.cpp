#include <cstdio>
bool __fastcall CCombatSequenceWhisperAttackCombo_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequenceWhisperAttackCombo_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4a50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4a50_TEST FAIL\n");
    return 1;
}