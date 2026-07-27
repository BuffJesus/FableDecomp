#include <cstdio>
bool __fastcall CCreatureAction_ReleaseAndLoopDivineWrathSpell_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleaseAndLoopDivineWrathSpell_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008bdc00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdc00_TEST FAIL\n");
    return 1;
}