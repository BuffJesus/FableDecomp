#include <cstdio>
bool __fastcall CCreatureAction_FodderUnblockablePrimaryAttack_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_FodderUnblockablePrimaryAttack_IsBlockable() == false) { std::printf("AUTO_TINY_00907420_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00907420_TEST FAIL\n");
    return 1;
}