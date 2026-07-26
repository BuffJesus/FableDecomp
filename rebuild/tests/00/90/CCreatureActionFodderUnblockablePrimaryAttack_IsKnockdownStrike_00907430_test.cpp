#include <cstdio>
bool __fastcall CCreatureAction_FodderUnblockablePrimaryAttack_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_FodderUnblockablePrimaryAttack_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_00907430_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00907430_TEST FAIL\n");
    return 1;
}