#include <cstdio>
bool __fastcall CCreatureAction_IceTrollIcicleAttack_IsStrikeAction()
{
    return true;
}
int main()
{
    if (CCreatureAction_IceTrollIcicleAttack_IsStrikeAction() == true) { std::printf("AUTO_TINY_00962960_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00962960_TEST FAIL\n");
    return 1;
}