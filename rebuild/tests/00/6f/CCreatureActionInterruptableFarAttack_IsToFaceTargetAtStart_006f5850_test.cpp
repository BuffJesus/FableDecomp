#include <cstdio>
bool __fastcall CCreatureAction_InterruptableFarAttack_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_InterruptableFarAttack_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_006f5850_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5850_TEST FAIL\n");
    return 1;
}