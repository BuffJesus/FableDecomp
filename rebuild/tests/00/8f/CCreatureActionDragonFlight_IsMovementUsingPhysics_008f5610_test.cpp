#include <cstdio>
bool __fastcall CCreatureAction_DragonFlight_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonFlight_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_008f5610_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5610_TEST FAIL\n");
    return 1;
}