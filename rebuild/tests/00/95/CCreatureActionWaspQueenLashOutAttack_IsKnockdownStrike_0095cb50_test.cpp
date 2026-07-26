#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenLashOutAttack_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_WaspQueenLashOutAttack_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_0095cb50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095cb50_TEST FAIL\n");
    return 1;
}