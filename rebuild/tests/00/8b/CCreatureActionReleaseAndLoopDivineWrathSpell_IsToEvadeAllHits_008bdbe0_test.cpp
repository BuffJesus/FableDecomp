#include <cstdio>
bool __fastcall CCreatureAction_ReleaseAndLoopDivineWrathSpell_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_ReleaseAndLoopDivineWrathSpell_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008bdbe0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdbe0_TEST FAIL\n");
    return 1;
}