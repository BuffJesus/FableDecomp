#include <cstdio>
bool __fastcall CCreatureAction_BanditKingAdvance_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_BanditKingAdvance_IsToFaceTarget() == true) { std::printf("AUTO_TINY_00906c00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00906c00_TEST FAIL\n");
    return 1;
}