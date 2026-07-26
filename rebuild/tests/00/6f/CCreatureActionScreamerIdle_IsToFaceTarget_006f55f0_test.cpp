#include <cstdio>
bool __fastcall CCreatureAction_ScreamerIdle_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScreamerIdle_IsToFaceTarget() == true) { std::printf("AUTO_TINY_006f55f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f55f0_TEST FAIL\n");
    return 1;
}