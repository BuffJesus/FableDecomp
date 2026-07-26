#include <cstdio>
bool __fastcall CCreatureAction_ScreamerBackOff_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScreamerBackOff_IsToFaceTarget() == true) { std::printf("AUTO_TINY_006f5670_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5670_TEST FAIL\n");
    return 1;
}