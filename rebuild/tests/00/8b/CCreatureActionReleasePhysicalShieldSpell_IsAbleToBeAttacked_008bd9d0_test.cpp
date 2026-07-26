#include <cstdio>
bool __fastcall CCreatureAction_ReleasePhysicalShieldSpell_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleasePhysicalShieldSpell_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008bd9d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bd9d0_TEST FAIL\n");
    return 1;
}