#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenLashOutAttack_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_WaspQueenLashOutAttack_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_0095cb30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095cb30_TEST FAIL\n");
    return 1;
}