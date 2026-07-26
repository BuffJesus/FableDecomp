#include <cstdio>
bool __fastcall CCreatureAction_TrollSwipe_IsPartOfComboChain()
{
    return true;
}
int main()
{
    if (CCreatureAction_TrollSwipe_IsPartOfComboChain() == true) { std::printf("AUTO_TINY_00908260_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00908260_TEST FAIL\n");
    return 1;
}