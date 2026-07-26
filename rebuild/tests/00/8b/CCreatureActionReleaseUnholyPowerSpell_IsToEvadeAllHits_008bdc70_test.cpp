#include <cstdio>
bool __fastcall CCreatureAction_ReleaseUnholyPowerSpell_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_ReleaseUnholyPowerSpell_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008bdc70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdc70_TEST FAIL\n");
    return 1;
}