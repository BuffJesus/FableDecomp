#include <cstdio>
int __fastcall CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_GetAnimDelayTime()
{
    return 0;
}
int main()
{
    if (CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_GetAnimDelayTime() == 0) { std::printf("AUTO_TINY_008c3ae0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c3ae0_TEST FAIL\n");
    return 1;
}