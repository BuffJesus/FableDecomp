#include <cstdio>
bool __fastcall CCreatureAction_GetOutOfBed_IsUsingAnimationMovement()
{
    return true;
}
int main()
{
    if (CCreatureAction_GetOutOfBed_IsUsingAnimationMovement() == true) { std::printf("AUTO_TINY_008600e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008600e0_TEST FAIL\n");
    return 1;
}