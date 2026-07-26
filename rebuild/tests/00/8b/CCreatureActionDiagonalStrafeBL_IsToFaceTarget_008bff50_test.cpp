#include <cstdio>
bool __fastcall CCreatureAction_DiagonalStrafeBL_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_DiagonalStrafeBL_IsToFaceTarget() == true) { std::printf("AUTO_TINY_008bff50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff50_TEST FAIL\n");
    return 1;
}