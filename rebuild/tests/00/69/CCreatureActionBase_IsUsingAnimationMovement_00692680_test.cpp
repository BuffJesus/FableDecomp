#include <cstdio>
bool __fastcall CCreatureActionBase_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_00692680_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692680_TEST FAIL\n");
    return 1;
}