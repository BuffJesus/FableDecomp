#include <cstdio>
bool __fastcall CCombatSequenceWhisperAttackBigCombo_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequenceWhisperAttackBigCombo_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4ad0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4ad0_TEST FAIL\n");
    return 1;
}