#include <cstdio>
bool __fastcall CCreatureAction_DiagonalStrafeBR_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_DiagonalStrafeBR_IsToFaceTarget() == true) { std::printf("AUTO_TINY_008bff60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff60_TEST FAIL\n");
    return 1;
}