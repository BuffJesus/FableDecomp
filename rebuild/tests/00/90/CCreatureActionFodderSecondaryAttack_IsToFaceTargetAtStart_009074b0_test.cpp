#include <cstdio>
bool __fastcall CCreatureAction_FodderSecondaryAttack_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_FodderSecondaryAttack_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_009074b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009074b0_TEST FAIL\n");
    return 1;
}