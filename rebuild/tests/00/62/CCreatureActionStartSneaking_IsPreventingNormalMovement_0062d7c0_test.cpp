#include <cstdio>
bool __fastcall CCreatureAction_StartSneaking_IsPreventingNormalMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_StartSneaking_IsPreventingNormalMovement() == false) { std::printf("AUTO_TINY_0062d7c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062d7c0_TEST FAIL\n");
    return 1;
}