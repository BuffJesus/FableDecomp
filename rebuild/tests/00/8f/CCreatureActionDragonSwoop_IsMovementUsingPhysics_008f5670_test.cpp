#include <cstdio>
bool __fastcall CCreatureAction_DragonSwoop_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonSwoop_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_008f5670_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5670_TEST FAIL\n");
    return 1;
}