#include <cstdio>
bool __fastcall CCreatureAction_NymphIdleAfterShot_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_NymphIdleAfterShot_IsToFaceTarget() == true) { std::printf("AUTO_TINY_008ca910_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ca910_TEST FAIL\n");
    return 1;
}