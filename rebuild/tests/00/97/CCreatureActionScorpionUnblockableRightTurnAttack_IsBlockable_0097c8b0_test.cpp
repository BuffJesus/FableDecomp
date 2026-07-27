#include <cstdio>
bool __fastcall CCreatureAction_ScorpionUnblockableRightTurnAttack_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionUnblockableRightTurnAttack_IsBlockable() == false) { std::printf("AUTO_TINY_0097c8b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0097c8b0_TEST FAIL\n");
    return 1;
}