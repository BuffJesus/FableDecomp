#include <cstdio>
bool __fastcall CCreatureAction_HeroLoadRangedWeapon_IsPreventingNormalMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_HeroLoadRangedWeapon_IsPreventingNormalMovement() == false) { std::printf("AUTO_TINY_0078aee0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0078aee0_TEST FAIL\n");
    return 1;
}