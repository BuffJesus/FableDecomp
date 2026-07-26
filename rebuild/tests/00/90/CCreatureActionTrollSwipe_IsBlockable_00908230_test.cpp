#include <cstdio>
bool __fastcall CCreatureAction_TrollSwipe_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_TrollSwipe_IsBlockable() == false) { std::printf("AUTO_TINY_00908230_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00908230_TEST FAIL\n");
    return 1;
}