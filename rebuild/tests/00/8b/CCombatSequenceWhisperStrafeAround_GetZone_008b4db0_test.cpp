#include <cstdio>
int __fastcall CCombatSequenceWhisperStrafeAround_GetZone()
{
    return 3;
}
int main()
{
    if (CCombatSequenceWhisperStrafeAround_GetZone() == 3) { std::printf("AUTO_TINY_008b4db0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4db0_TEST FAIL\n");
    return 1;
}