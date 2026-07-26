#include <cstdio>
bool __fastcall CCreatureAction_LeapLong_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_LeapLong_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008b86b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b86b0_TEST FAIL\n");
    return 1;
}