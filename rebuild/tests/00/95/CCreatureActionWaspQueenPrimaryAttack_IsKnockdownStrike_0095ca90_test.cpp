#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenPrimaryAttack_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_WaspQueenPrimaryAttack_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_0095ca90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095ca90_TEST FAIL\n");
    return 1;
}