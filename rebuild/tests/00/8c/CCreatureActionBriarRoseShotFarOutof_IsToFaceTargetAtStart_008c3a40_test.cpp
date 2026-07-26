#include <cstdio>
bool __fastcall CCreatureAction_BriarRoseShotFarOutof_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_BriarRoseShotFarOutof_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008c3a40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c3a40_TEST FAIL\n");
    return 1;
}