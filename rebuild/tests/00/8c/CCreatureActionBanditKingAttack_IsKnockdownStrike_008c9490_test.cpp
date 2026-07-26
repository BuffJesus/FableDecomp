#include <cstdio>
bool __fastcall CCreatureAction_BanditKingAttack_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_BanditKingAttack_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_008c9490_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c9490_TEST FAIL\n");
    return 1;
}