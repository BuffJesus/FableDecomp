#include <cstdio>
bool __fastcall CCombatAction_GenericStrikeResponseKnockdown_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCombatAction_GenericStrikeResponseKnockdown_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_00865b10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00865b10_TEST FAIL\n");
    return 1;
}