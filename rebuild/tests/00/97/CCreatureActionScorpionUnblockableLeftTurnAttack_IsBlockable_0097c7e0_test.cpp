#include <cstdio>
bool __fastcall CCreatureAction_ScorpionUnblockableLeftTurnAttack_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionUnblockableLeftTurnAttack_IsBlockable() == false) { std::printf("AUTO_TINY_0097c7e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0097c7e0_TEST FAIL\n");
    return 1;
}