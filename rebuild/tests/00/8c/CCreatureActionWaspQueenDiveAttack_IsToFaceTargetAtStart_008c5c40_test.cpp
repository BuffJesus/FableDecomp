#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenDiveAttack_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_WaspQueenDiveAttack_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008c5c40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5c40_TEST FAIL\n");
    return 1;
}