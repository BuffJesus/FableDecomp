#include <cstdio>
bool __fastcall CCreatureAction_UnblockableMidAttack_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_UnblockableMidAttack_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_008bff30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff30_TEST FAIL\n");
    return 1;
}