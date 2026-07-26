#include <cstdio>
int __fastcall CCombatSequence_WillEnflame_GetZone()
{
    return 3;
}
int main()
{
    if (CCombatSequence_WillEnflame_GetZone() == 3) { std::printf("AUTO_TINY_008b5200_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5200_TEST FAIL\n");
    return 1;
}