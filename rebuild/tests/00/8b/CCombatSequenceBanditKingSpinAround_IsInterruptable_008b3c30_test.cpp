#include <cstdio>
bool __fastcall CCombatSequence_BanditKingSpinAround_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_BanditKingSpinAround_IsInterruptable() == false) { std::printf("AUTO_TINY_008b3c30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3c30_TEST FAIL\n");
    return 1;
}