#include <cstdio>
bool __fastcall CCreatureAction_BriarRoseShotFarOutof_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_BriarRoseShotFarOutof_IsToFaceTarget() == true) { std::printf("AUTO_TINY_008c3a30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c3a30_TEST FAIL\n");
    return 1;
}