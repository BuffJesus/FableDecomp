#include <cstdio>
bool __fastcall CCombatSequence_BanditKingAttack_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_BanditKingAttack_IsInterruptable() == false) { std::printf("AUTO_TINY_008b3ab0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3ab0_TEST FAIL\n");
    return 1;
}