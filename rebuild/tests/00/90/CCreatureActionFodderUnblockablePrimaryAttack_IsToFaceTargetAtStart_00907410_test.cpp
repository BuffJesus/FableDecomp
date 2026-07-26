#include <cstdio>
bool __fastcall CCreatureAction_FodderUnblockablePrimaryAttack_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_FodderUnblockablePrimaryAttack_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_00907410_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00907410_TEST FAIL\n");
    return 1;
}