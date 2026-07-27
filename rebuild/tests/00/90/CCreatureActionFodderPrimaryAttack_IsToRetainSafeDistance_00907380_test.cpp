#include <cstdio>
bool __fastcall CCreatureAction_FodderPrimaryAttack_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_FodderPrimaryAttack_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_00907380_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00907380_TEST FAIL\n");
    return 1;
}