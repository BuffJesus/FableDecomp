#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionInsult_IsToReplaceItemIfRemoved()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionInsult_IsToReplaceItemIfRemoved() == true) { std::printf("AUTO_TINY_0062dc60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062dc60_TEST FAIL\n");
    return 1;
}