#include <cstdio>
bool __fastcall CCreatureAction_BriarRoseIntoInvulnerable_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_BriarRoseIntoInvulnerable_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_00957270_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00957270_TEST FAIL\n");
    return 1;
}