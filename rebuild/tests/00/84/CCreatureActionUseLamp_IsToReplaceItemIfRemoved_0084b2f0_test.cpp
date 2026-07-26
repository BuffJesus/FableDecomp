#include <cstdio>
bool __fastcall CCreatureAction_UseLamp_IsToReplaceItemIfRemoved()
{
    return true;
}
int main()
{
    if (CCreatureAction_UseLamp_IsToReplaceItemIfRemoved() == true) { std::printf("AUTO_TINY_0084b2f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0084b2f0_TEST FAIL\n");
    return 1;
}