#include <cstdio>
bool __fastcall CCreatureAction_BuildUpBattleCharge_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_BuildUpBattleCharge_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008cec10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cec10_TEST FAIL\n");
    return 1;
}