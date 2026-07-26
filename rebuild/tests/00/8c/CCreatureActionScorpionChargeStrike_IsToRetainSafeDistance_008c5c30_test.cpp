#include <cstdio>
bool __fastcall CCreatureAction_ScorpionChargeStrike_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionChargeStrike_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_008c5c30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5c30_TEST FAIL\n");
    return 1;
}