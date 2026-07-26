#include <cstdio>
int __fastcall CCombatSequence_TrollDoubleSwipe_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequence_TrollDoubleSwipe_GetZone() == 4) { std::printf("AUTO_TINY_008b4010_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4010_TEST FAIL\n");
    return 1;
}