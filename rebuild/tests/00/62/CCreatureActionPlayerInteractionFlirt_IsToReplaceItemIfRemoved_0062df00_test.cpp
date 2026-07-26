#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionFlirt_IsToReplaceItemIfRemoved()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionFlirt_IsToReplaceItemIfRemoved() == true) { std::printf("AUTO_TINY_0062df00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062df00_TEST FAIL\n");
    return 1;
}