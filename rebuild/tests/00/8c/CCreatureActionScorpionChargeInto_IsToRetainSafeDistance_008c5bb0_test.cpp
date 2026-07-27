#include <cstdio>
bool __fastcall CCreatureAction_ScorpionChargeInto_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionChargeInto_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_008c5bb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5bb0_TEST FAIL\n");
    return 1;
}