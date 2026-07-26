#include <cstdio>
bool __fastcall CCreatureAction_TrollThrowProjectile_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_TrollThrowProjectile_IsToFaceTarget() == true) { std::printf("AUTO_TINY_008c7cd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c7cd0_TEST FAIL\n");
    return 1;
}