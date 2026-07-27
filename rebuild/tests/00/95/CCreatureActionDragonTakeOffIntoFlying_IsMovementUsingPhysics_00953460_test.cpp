#include <cstdio>
bool __fastcall CCreatureAction_DragonTakeOffIntoFlying_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonTakeOffIntoFlying_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_00953460_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00953460_TEST FAIL\n");
    return 1;
}