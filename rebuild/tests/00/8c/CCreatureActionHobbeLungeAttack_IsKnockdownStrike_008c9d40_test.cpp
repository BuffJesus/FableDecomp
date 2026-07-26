#include <cstdio>
bool __fastcall CCreatureAction_HobbeLungeAttack_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_HobbeLungeAttack_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_008c9d40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c9d40_TEST FAIL\n");
    return 1;
}