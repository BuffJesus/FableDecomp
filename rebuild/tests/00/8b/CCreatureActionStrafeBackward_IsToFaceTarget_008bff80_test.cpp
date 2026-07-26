#include <cstdio>
bool __fastcall CCreatureAction_StrafeBackward_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_StrafeBackward_IsToFaceTarget() == true) { std::printf("AUTO_TINY_008bff80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff80_TEST FAIL\n");
    return 1;
}