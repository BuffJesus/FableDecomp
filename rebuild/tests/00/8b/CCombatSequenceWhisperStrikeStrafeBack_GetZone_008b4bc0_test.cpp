#include <cstdio>
int __fastcall CCombatSequenceWhisperStrikeStrafeBack_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequenceWhisperStrikeStrafeBack_GetZone() == 4) { std::printf("AUTO_TINY_008b4bc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4bc0_TEST FAIL\n");
    return 1;
}