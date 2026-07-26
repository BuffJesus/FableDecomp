#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionBribe_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionBribe_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_0062de10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062de10_TEST FAIL\n");
    return 1;
}