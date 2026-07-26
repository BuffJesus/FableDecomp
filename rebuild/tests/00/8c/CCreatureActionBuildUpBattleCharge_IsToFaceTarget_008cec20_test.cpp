#include <cstdio>
bool __fastcall CCreatureAction_BuildUpBattleCharge_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_BuildUpBattleCharge_IsToFaceTarget() == true) { std::printf("AUTO_TINY_008cec20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cec20_TEST FAIL\n");
    return 1;
}