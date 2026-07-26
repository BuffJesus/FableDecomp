#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenPrimaryAttack_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_WaspQueenPrimaryAttack_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_0095ca70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095ca70_TEST FAIL\n");
    return 1;
}