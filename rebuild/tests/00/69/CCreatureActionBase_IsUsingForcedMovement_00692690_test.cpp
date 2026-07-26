#include <cstdio>
bool __fastcall CCreatureActionBase_IsUsingForcedMovement()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsUsingForcedMovement() == false) { std::printf("AUTO_TINY_00692690_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692690_TEST FAIL\n");
    return 1;
}