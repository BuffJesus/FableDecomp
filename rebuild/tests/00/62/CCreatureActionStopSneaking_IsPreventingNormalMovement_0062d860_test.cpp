#include <cstdio>
bool __fastcall CCreatureAction_StopSneaking_IsPreventingNormalMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_StopSneaking_IsPreventingNormalMovement() == false) { std::printf("AUTO_TINY_0062d860_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062d860_TEST FAIL\n");
    return 1;
}