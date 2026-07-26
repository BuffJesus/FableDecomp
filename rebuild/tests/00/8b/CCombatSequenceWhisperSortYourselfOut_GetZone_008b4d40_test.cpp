#include <cstdio>
int __fastcall CCombatSequenceWhisperSortYourselfOut_GetZone()
{
    return 64;
}
int main()
{
    if (CCombatSequenceWhisperSortYourselfOut_GetZone() == 64) { std::printf("AUTO_TINY_008b4d40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4d40_TEST FAIL\n");
    return 1;
}