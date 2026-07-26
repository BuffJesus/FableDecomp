#include <cstdio>
bool __fastcall CCreatureAction_BanditKingLunge_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_BanditKingLunge_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_00961a10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00961a10_TEST FAIL\n");
    return 1;
}