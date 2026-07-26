#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionGreet_IsPreventingLookTurning()
{
    return false;
}
int main()
{
    if (CCreatureAction_PlayerInteractionGreet_IsPreventingLookTurning() == false) { std::printf("AUTO_TINY_0062db60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062db60_TEST FAIL\n");
    return 1;
}