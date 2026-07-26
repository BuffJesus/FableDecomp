#include <cstdio>
bool __fastcall CCreatureAction_ScreamerAdvance_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_ScreamerAdvance_IsToFaceTarget() == true) { std::printf("AUTO_TINY_006f5630_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5630_TEST FAIL\n");
    return 1;
}