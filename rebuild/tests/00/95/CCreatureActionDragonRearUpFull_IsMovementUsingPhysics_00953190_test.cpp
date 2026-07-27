#include <cstdio>
bool __fastcall CCreatureAction_DragonRearUpFull_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonRearUpFull_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_00953190_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00953190_TEST FAIL\n");
    return 1;
}