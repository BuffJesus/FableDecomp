#include <cstdio>
bool __fastcall CCreatureAction_BlockBase_IsToFaceTarget()
{
    return false;
}
int main()
{
    if (CCreatureAction_BlockBase_IsToFaceTarget() == false) { std::printf("AUTO_TINY_00855d00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00855d00_TEST FAIL\n");
    return 1;
}