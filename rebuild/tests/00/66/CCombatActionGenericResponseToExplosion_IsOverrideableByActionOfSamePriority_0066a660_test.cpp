#include <cstdio>
bool __fastcall CCombatAction_GenericResponseToExplosion_IsOverrideableByActionOfSamePriority()
{
    return true;
}
int main()
{
    if (CCombatAction_GenericResponseToExplosion_IsOverrideableByActionOfSamePriority() == true) { std::printf("AUTO_TINY_0066a660_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0066a660_TEST FAIL\n");
    return 1;
}