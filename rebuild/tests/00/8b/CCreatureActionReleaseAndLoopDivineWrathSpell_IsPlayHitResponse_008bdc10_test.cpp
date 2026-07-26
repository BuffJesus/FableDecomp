#include <cstdio>
bool __fastcall CCreatureAction_ReleaseAndLoopDivineWrathSpell_IsPlayHitResponse()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleaseAndLoopDivineWrathSpell_IsPlayHitResponse() == false) { std::printf("AUTO_TINY_008bdc10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdc10_TEST FAIL\n");
    return 1;
}