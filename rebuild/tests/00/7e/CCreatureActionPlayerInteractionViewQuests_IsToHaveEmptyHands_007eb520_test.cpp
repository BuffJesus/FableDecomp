#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionViewQuests_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionViewQuests_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_007eb520_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007eb520_TEST FAIL\n");
    return 1;
}