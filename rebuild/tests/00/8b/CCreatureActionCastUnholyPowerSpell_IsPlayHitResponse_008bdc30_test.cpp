#include <cstdio>
bool __fastcall CCreatureAction_CastUnholyPowerSpell_IsPlayHitResponse()
{
    return false;
}
int main()
{
    if (CCreatureAction_CastUnholyPowerSpell_IsPlayHitResponse() == false) { std::printf("AUTO_TINY_008bdc30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdc30_TEST FAIL\n");
    return 1;
}