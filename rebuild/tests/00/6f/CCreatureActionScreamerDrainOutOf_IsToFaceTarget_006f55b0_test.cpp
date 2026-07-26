#include <cstdio>
bool __fastcall CCreatureAction_ScreamerDrainOutOf_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScreamerDrainOutOf_IsToFaceTarget() == true) { std::printf("AUTO_TINY_006f55b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f55b0_TEST FAIL\n");
    return 1;
}