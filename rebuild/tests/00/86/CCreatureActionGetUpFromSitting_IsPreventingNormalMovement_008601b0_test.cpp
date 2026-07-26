#include <cstdio>
bool __fastcall CCreatureAction_GetUpFromSitting_IsPreventingNormalMovement()
{
    return true;
}
int main()
{
    if (CCreatureAction_GetUpFromSitting_IsPreventingNormalMovement() == true) { std::printf("AUTO_TINY_008601b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008601b0_TEST FAIL\n");
    return 1;
}