#include <cstdio>
bool __fastcall CCreatureAction_MidAttackFacing_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_MidAttackFacing_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_0090a6f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0090a6f0_TEST FAIL\n");
    return 1;
}