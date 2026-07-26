#include <cstdio>
bool __fastcall CCreatureAction_PlayerInteractionInsult_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_PlayerInteractionInsult_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_0062dc50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062dc50_TEST FAIL\n");
    return 1;
}