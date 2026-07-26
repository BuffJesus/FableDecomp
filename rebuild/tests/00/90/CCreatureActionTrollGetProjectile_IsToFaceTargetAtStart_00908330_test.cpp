#include <cstdio>
bool __fastcall CCreatureAction_TrollGetProjectile_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_TrollGetProjectile_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_00908330_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00908330_TEST FAIL\n");
    return 1;
}