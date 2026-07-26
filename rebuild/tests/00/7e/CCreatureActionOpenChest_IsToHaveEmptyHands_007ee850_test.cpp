#include <cstdio>
bool __fastcall CCreatureAction_OpenChest_IsToHaveEmptyHands()
{
    return true;
}
int main()
{
    if (CCreatureAction_OpenChest_IsToHaveEmptyHands() == true) { std::printf("AUTO_TINY_007ee850_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007ee850_TEST FAIL\n");
    return 1;
}