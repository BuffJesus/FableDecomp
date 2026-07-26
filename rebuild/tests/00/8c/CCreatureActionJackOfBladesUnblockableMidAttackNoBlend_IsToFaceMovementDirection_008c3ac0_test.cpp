#include <cstdio>
bool __fastcall CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_IsToFaceMovementDirection()
{
    return false;
}
int main()
{
    if (CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_IsToFaceMovementDirection() == false) { std::printf("AUTO_TINY_008c3ac0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c3ac0_TEST FAIL\n");
    return 1;
}