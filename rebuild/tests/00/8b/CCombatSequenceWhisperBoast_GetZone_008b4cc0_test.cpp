#include <cstdio>
int __fastcall CCombatSequenceWhisperBoast_GetZone()
{
    return 3;
}
int main()
{
    if (CCombatSequenceWhisperBoast_GetZone() == 3) { std::printf("AUTO_TINY_008b4cc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4cc0_TEST FAIL\n");
    return 1;
}