#include <cstdio>
int __fastcall CCombatSequenceJacksCombo_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequenceJacksCombo_GetZone() == 2) { std::printf("AUTO_TINY_008b4eb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4eb0_TEST FAIL\n");
    return 1;
}