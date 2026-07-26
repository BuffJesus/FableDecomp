#include <cstdio>
bool __fastcall CCreatureAction_ScreamerDrainAttack_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScreamerDrainAttack_IsToFaceTarget() == true) { std::printf("AUTO_TINY_006f5560_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5560_TEST FAIL\n");
    return 1;
}