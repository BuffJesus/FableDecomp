#include <cstdio>
bool __fastcall CCreatureAction_GenericFireProjectileWeapon_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_GenericFireProjectileWeapon_IsToFaceTarget() == true) { std::printf("AUTO_TINY_006f5ae0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5ae0_TEST FAIL\n");
    return 1;
}