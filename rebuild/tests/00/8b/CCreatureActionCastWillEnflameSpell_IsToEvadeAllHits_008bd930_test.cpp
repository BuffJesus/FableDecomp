#include <cstdio>
bool __fastcall CCreatureAction_CastWillEnflameSpell_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_CastWillEnflameSpell_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008bd930_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bd930_TEST FAIL\n");
    return 1;
}