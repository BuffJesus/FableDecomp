#include <cstdio>
int __fastcall CCombatSequence_FodderCombatIdle_GetZone()
{
    return 7;
}
int main()
{
    if (CCombatSequence_FodderCombatIdle_GetZone() == 7) { std::printf("AUTO_TINY_008b3e20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3e20_TEST FAIL\n");
    return 1;
}