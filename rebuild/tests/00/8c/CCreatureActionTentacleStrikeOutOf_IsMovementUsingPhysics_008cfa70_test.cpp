#include <cstdio>
bool __fastcall CCreatureAction_TentacleStrikeOutOf_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_TentacleStrikeOutOf_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_008cfa70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cfa70_TEST FAIL\n");
    return 1;
}