#include <cstdio>
bool __fastcall CCreatureAction_DragonRetreat_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonRetreat_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_008f5640_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008f5640_TEST FAIL\n");
    return 1;
}