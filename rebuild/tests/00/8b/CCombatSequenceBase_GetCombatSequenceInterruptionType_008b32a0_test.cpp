#include <cstdio>
int __fastcall CCombatSequenceBase_GetCombatSequenceInterruptionType()
{
    return 0;
}
int main()
{
    if (CCombatSequenceBase_GetCombatSequenceInterruptionType() == 0) { std::printf("AUTO_TINY_008b32a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b32a0_TEST FAIL\n");
    return 1;
}