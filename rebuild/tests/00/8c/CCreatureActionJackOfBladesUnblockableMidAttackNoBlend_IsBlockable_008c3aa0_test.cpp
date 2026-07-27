#include <cstdio>
bool __fastcall CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_IsBlockable() == false) { std::printf("AUTO_TINY_008c3aa0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c3aa0_TEST FAIL\n");
    return 1;
}