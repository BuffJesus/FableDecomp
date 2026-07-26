#include <cstdio>
bool __fastcall CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_008c3ab0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c3ab0_TEST FAIL\n");
    return 1;
}