#include <cstdio>
bool __fastcall CCreatureAction_CastDivineWrathSpell_IsPlayHitResponse()
{
    return false;
}
int main()
{
    if (CCreatureAction_CastDivineWrathSpell_IsPlayHitResponse() == false) { std::printf("AUTO_TINY_008bdb50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdb50_TEST FAIL\n");
    return 1;
}