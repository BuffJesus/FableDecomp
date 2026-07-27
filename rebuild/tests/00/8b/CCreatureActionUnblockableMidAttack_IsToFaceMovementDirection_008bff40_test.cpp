#include <cstdio>
bool __fastcall CCreatureAction_UnblockableMidAttack_IsToFaceMovementDirection()
{
    return false;
}
int main()
{
    if (CCreatureAction_UnblockableMidAttack_IsToFaceMovementDirection() == false) { std::printf("AUTO_TINY_008bff40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff40_TEST FAIL\n");
    return 1;
}