#include <cstdio>
bool __fastcall CCombatSequenceWhisperTooCloseLeapBack_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequenceWhisperTooCloseLeapBack_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4c50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4c50_TEST FAIL\n");
    return 1;
}