#include <cstdio>
bool __fastcall CCreatureAction_OpenChest_IsToReplaceItemIfRemoved()
{
    return true;
}
int main()
{
    if (CCreatureAction_OpenChest_IsToReplaceItemIfRemoved() == true) { std::printf("AUTO_TINY_007ee860_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007ee860_TEST FAIL\n");
    return 1;
}