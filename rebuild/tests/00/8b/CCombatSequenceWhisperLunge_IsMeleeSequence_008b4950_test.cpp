#include <cstdio>
bool __fastcall CCombatSequenceWhisperLunge_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequenceWhisperLunge_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4950_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4950_TEST FAIL\n");
    return 1;
}