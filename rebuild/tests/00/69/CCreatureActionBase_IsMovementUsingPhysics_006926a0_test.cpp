#include <cstdio>
bool __fastcall CCreatureActionBase_IsMovementUsingPhysics()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsMovementUsingPhysics() == false) { std::printf("AUTO_TINY_006926a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006926a0_TEST FAIL\n");
    return 1;
}