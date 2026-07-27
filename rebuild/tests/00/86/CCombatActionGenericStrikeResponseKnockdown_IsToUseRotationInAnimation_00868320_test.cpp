#include <cstdio>
bool __fastcall CCombatAction_GenericStrikeResponseKnockdown_IsToUseRotationInAnimation()
{
    return false;
}
int main()
{
    if (CCombatAction_GenericStrikeResponseKnockdown_IsToUseRotationInAnimation() == false) { std::printf("AUTO_TINY_00868320_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00868320_TEST FAIL\n");
    return 1;
}