#include <cstdio>
bool __fastcall CCreatureAction_BalverineLungeAttack_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_BalverineLungeAttack_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_008cd230_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cd230_TEST FAIL\n");
    return 1;
}