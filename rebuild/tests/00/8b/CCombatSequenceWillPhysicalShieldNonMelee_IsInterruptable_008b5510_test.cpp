#include <cstdio>
bool __fastcall CCombatSequence_WillPhysicalShieldNonMelee_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillPhysicalShieldNonMelee_IsInterruptable() == false) { std::printf("AUTO_TINY_008b5510_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5510_TEST FAIL\n");
    return 1;
}