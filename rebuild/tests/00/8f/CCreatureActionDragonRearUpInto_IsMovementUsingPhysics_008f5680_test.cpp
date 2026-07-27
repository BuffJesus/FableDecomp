#include <cstdio>
bool __fastcall CCreatureAction_DragonRearUpInto_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonRearUpInto_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_008f5680_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5680_TEST FAIL\n");
    return 1;
}