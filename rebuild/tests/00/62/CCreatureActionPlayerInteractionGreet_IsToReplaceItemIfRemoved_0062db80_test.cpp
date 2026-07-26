#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionGreet_IsToReplaceItemIfRemoved()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionGreet_IsToReplaceItemIfRemoved() == true) { std::printf("AUTO_TINY_0062db80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062db80_TEST FAIL\n");
    return 1;
}