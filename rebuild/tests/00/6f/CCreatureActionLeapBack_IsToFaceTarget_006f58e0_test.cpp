#include <cstdio>
bool __fastcall CCreatureAction_LeapBack_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_LeapBack_IsToFaceTarget() == true) { std::printf("AUTO_TINY_006f58e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f58e0_TEST FAIL\n");
    return 1;
}