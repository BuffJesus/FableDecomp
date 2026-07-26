#include <cstdio>
bool __fastcall CCreatureAction_BanditKingLunge_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_BanditKingLunge_IsBlockable() == false) { std::printf("AUTO_TINY_00961a20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00961a20_TEST FAIL\n");
    return 1;
}