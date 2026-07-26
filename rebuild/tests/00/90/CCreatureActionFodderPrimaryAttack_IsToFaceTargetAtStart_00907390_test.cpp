#include <cstdio>
bool __fastcall CCreatureAction_FodderPrimaryAttack_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_FodderPrimaryAttack_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_00907390_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00907390_TEST FAIL\n");
    return 1;
}