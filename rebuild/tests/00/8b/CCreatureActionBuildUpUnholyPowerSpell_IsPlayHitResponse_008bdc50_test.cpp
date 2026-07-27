#include <cstdio>
bool __fastcall CCreatureAction_BuildUpUnholyPowerSpell_IsPlayHitResponse()
{
    return false;
}
int main()
{
    if (CCreatureAction_BuildUpUnholyPowerSpell_IsPlayHitResponse() == false) { std::printf("AUTO_TINY_008bdc50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdc50_TEST FAIL\n");
    return 1;
}