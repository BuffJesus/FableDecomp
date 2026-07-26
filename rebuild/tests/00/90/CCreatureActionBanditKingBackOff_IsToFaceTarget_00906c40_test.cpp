#include <cstdio>
bool __fastcall CCreatureAction_BanditKingBackOff_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_BanditKingBackOff_IsToFaceTarget() == true) { std::printf("AUTO_TINY_00906c40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00906c40_TEST FAIL\n");
    return 1;
}