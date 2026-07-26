#include <cstdio>
bool __fastcall CCreatureAction_GetInBed_IsUsingAnimationMovement()
{
    return true;
}
int main()
{
    if (CCreatureAction_GetInBed_IsUsingAnimationMovement() == true) { std::printf("AUTO_TINY_0092e3d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0092e3d0_TEST FAIL\n");
    return 1;
}