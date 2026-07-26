#include <cstdio>
bool __fastcall CCreatureAction_InterruptableMidAttackAutoTurn_IsToPerformActionOnStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_InterruptableMidAttackAutoTurn_IsToPerformActionOnStrike() == true) { std::printf("AUTO_TINY_0087e510_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0087e510_TEST FAIL\n");
    return 1;
}