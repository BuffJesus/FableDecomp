#include <cstdio>
bool __fastcall CCreatureAction_Ambush_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureAction_Ambush_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_008bff90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff90_TEST FAIL\n");
    return 1;
}