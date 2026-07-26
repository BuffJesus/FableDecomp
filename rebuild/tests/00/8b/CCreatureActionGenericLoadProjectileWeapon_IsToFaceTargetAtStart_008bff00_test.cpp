#include <cstdio>
bool __fastcall CCreatureAction_GenericLoadProjectileWeapon_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_GenericLoadProjectileWeapon_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008bff00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff00_TEST FAIL\n");
    return 1;
}