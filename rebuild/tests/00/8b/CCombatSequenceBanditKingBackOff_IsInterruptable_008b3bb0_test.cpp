#include <cstdio>
bool __fastcall CCombatSequence_BanditKingBackOff_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_BanditKingBackOff_IsInterruptable() == false) { std::printf("AUTO_TINY_008b3bb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3bb0_TEST FAIL\n");
    return 1;
}