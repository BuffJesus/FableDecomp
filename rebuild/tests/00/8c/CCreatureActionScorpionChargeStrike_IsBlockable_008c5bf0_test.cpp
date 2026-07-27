#include <cstdio>
bool __fastcall CCreatureAction_ScorpionChargeStrike_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionChargeStrike_IsBlockable() == false) { std::printf("AUTO_TINY_008c5bf0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5bf0_TEST FAIL\n");
    return 1;
}