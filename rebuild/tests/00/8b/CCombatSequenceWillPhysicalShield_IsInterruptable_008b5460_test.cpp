#include <cstdio>
bool __fastcall CCombatSequence_WillPhysicalShield_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillPhysicalShield_IsInterruptable() == false) { std::printf("AUTO_TINY_008b5460_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5460_TEST FAIL\n");
    return 1;
}