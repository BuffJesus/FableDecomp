#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenPrimaryAttack_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_WaspQueenPrimaryAttack_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_0095ca60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095ca60_TEST FAIL\n");
    return 1;
}