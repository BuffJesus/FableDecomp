#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionFlirt_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionFlirt_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_0062def0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062def0_TEST FAIL\n");
    return 1;
}