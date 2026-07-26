#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionGreet_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionGreet_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_0062db70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062db70_TEST FAIL\n");
    return 1;
}