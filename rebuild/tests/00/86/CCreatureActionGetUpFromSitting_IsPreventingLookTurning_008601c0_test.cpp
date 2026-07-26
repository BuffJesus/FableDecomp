#include <cstdio>
bool __fastcall CCreatureAction_GetUpFromSitting_IsPreventingLookTurning()
{
    return true;
}
int main()
{
    if (CCreatureAction_GetUpFromSitting_IsPreventingLookTurning() == true) { std::printf("AUTO_TINY_008601c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008601c0_TEST FAIL\n");
    return 1;
}