#include <cstdio>
bool __fastcall CCombatSequence_ScorpionLungeAttack_IsToPreferSequenceWhenValid()
{
    return true;
}
int main()
{
    if (CCombatSequence_ScorpionLungeAttack_IsToPreferSequenceWhenValid() == true) { std::printf("AUTO_TINY_008b3ec0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3ec0_TEST FAIL\n");
    return 1;
}