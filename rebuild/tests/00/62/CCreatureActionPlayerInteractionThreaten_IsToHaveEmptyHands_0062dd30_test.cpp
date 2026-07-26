#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionThreaten_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionThreaten_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_0062dd30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062dd30_TEST FAIL\n");
    return 1;
}