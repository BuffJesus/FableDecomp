#include <cstdio>
bool __fastcall CCreatureAction_BuildUpDivineWrathSpell_IsPlayHitResponse()
{
    return false;
}
int main()
{
    if (CCreatureAction_BuildUpDivineWrathSpell_IsPlayHitResponse() == false) { std::printf("AUTO_TINY_008bdb70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdb70_TEST FAIL\n");
    return 1;
}