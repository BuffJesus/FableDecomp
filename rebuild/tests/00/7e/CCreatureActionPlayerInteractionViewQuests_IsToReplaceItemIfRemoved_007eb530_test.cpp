#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionViewQuests_IsToReplaceItemIfRemoved()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionViewQuests_IsToReplaceItemIfRemoved() == true) { std::printf("AUTO_TINY_007eb530_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007eb530_TEST FAIL\n");
    return 1;
}