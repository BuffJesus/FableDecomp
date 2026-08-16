#include <cstdio>
int __fastcall CCombatSequenceBase_GetCombatSequenceType()
{
    return 0;
}
int main()
{
    if (CCombatSequenceBase_GetCombatSequenceType() == 0) { std::printf("AUTO_TINY_008b3290_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3290_TEST FAIL\n");
    return 1;
}