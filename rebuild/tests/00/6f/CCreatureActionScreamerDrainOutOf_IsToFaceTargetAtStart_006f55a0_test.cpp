#include <cstdio>
bool __fastcall CCreatureAction_ScreamerDrainOutOf_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScreamerDrainOutOf_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_006f55a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f55a0_TEST FAIL\n");
    return 1;
}