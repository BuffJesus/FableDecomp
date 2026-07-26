#include <cstdio>
bool __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_IsPreventingQuickAccessItemUsage()
{
    return true;
}
int main()
{
    if (CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_IsPreventingQuickAccessItemUsage() == true) { std::printf("AUTO_TINY_007f0630_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007f0630_TEST FAIL\n");
    return 1;
}