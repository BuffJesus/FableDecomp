#include <cstdio>
bool __fastcall CCombatActionBase_IsMovementUsingPhysics()
{
    return true;
}
int main()
{
    if (CCombatActionBase_IsMovementUsingPhysics() == true) { std::printf("AUTO_TINY_00856300_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00856300_TEST FAIL\n");
    return 1;
}