#include <cstdio>
bool __fastcall CCreatureAction_ScorpionCharge_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionCharge_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_008c5be0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5be0_TEST FAIL\n");
    return 1;
}