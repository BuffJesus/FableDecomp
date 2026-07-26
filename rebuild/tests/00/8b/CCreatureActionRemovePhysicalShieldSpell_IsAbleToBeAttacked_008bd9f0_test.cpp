#include <cstdio>
bool __fastcall CCreatureAction_RemovePhysicalShieldSpell_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_RemovePhysicalShieldSpell_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008bd9f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bd9f0_TEST FAIL\n");
    return 1;
}