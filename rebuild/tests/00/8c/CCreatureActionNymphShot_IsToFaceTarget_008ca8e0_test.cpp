#include <cstdio>
bool __fastcall CCreatureAction_NymphShot_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_NymphShot_IsToFaceTarget() == true) { std::printf("AUTO_TINY_008ca8e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ca8e0_TEST FAIL\n");
    return 1;
}