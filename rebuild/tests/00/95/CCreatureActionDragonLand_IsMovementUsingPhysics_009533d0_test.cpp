#include <cstdio>
bool __fastcall CCreatureAction_DragonLand_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonLand_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_009533d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009533d0_TEST FAIL\n");
    return 1;
}