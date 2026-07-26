#include <cstdio>
bool __fastcall CCreatureAction_InterruptableFarAttack_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_InterruptableFarAttack_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_006f5840_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5840_TEST FAIL\n");
    return 1;
}