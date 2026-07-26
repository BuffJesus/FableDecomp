#include <cstdio>
bool __fastcall CCreatureAction_StrafeForward_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_StrafeForward_IsToFaceTarget() == true) { std::printf("AUTO_TINY_008bff70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff70_TEST FAIL\n");
    return 1;
}