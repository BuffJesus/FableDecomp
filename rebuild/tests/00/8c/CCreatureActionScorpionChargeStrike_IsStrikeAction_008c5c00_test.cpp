#include <cstdio>
bool __fastcall CCreatureAction_ScorpionChargeStrike_IsStrikeAction()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScorpionChargeStrike_IsStrikeAction() == true) { std::printf("AUTO_TINY_008c5c00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5c00_TEST FAIL\n");
    return 1;
}