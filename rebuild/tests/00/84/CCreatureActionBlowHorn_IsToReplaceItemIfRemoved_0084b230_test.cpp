#include <cstdio>
bool __fastcall CCreatureAction_BlowHorn_IsToReplaceItemIfRemoved()
{
    return true;
}
int main()
{
    if (CCreatureAction_BlowHorn_IsToReplaceItemIfRemoved() == true) { std::printf("AUTO_TINY_0084b230_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0084b230_TEST FAIL\n");
    return 1;
}