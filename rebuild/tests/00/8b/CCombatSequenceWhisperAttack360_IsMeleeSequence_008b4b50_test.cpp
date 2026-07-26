#include <cstdio>
bool __fastcall CCombatSequenceWhisperAttack360_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequenceWhisperAttack360_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4b50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4b50_TEST FAIL\n");
    return 1;
}