#include <cstdio>
bool __fastcall CCreatureAction_HeroLoadRangedWeapon_IsAffectedByHaste()
{
    return true;
}
int main()
{
    if (CCreatureAction_HeroLoadRangedWeapon_IsAffectedByHaste() == true) { std::printf("AUTO_TINY_0078aef0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0078aef0_TEST FAIL\n");
    return 1;
}