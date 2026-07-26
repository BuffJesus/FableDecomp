#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionThreaten_IsToReplaceItemIfRemoved()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionThreaten_IsToReplaceItemIfRemoved() == true) { std::printf("AUTO_TINY_0062dd40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062dd40_TEST FAIL\n");
    return 1;
}