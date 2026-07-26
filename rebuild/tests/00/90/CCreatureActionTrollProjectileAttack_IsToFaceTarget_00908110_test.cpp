#include <cstdio>
bool __fastcall CCreatureAction_TrollProjectileAttack_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_TrollProjectileAttack_IsToFaceTarget() == true) { std::printf("AUTO_TINY_00908110_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00908110_TEST FAIL\n");
    return 1;
}