#include <cstdio>
int __fastcall CCombatSequenceWhisperTooCloseLeapBack_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequenceWhisperTooCloseLeapBack_GetZone() == 4) { std::printf("AUTO_TINY_008b4c40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4c40_TEST FAIL\n");
    return 1;
}