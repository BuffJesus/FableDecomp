#include <cstdio>
bool __fastcall CCreatureAction_ThrowShotObject_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_ThrowShotObject_IsToFaceTarget() == true) { std::printf("AUTO_TINY_00956720_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00956720_TEST FAIL\n");
    return 1;
}