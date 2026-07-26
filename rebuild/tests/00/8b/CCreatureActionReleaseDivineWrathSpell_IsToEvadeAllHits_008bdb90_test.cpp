#include <cstdio>
bool __fastcall CCreatureAction_ReleaseDivineWrathSpell_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_ReleaseDivineWrathSpell_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008bdb90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdb90_TEST FAIL\n");
    return 1;
}