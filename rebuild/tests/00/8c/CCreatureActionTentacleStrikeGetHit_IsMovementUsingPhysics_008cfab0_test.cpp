#include <cstdio>
bool __fastcall CCreatureAction_TentacleStrikeGetHit_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_TentacleStrikeGetHit_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_008cfab0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cfab0_TEST FAIL\n");
    return 1;
}