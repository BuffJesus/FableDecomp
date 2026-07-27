#include <cstdio>
bool __fastcall CCreatureAction_FodderUnblockablePrimaryAttack_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_FodderUnblockablePrimaryAttack_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_00907400_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00907400_TEST FAIL\n");
    return 1;
}