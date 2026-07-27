#include <cstdio>
bool __fastcall CCreatureAction_DragonRearUpOutOf_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonRearUpOutOf_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_008f5690_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5690_TEST FAIL\n");
    return 1;
}