#include <cstdio>
bool __fastcall CCombatSequenceWhisperStrikeStrafeBack_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequenceWhisperStrikeStrafeBack_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4bd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4bd0_TEST FAIL\n");
    return 1;
}