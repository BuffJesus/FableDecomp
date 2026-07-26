#include <cstdio>
bool __fastcall CCreatureAction_BanditKingThrowWeapons_IsToPerformActionOnStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_BanditKingThrowWeapons_IsToPerformActionOnStrike() == true) { std::printf("AUTO_TINY_008c93d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c93d0_TEST FAIL\n");
    return 1;
}