#include <cstdio>
bool __fastcall CCreatureAction_BalverineLungeAttack_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_BalverineLungeAttack_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008cd220_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cd220_TEST FAIL\n");
    return 1;
}