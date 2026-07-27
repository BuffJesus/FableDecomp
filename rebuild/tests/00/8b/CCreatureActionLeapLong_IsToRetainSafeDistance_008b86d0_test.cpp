#include <cstdio>
bool __fastcall CCreatureAction_LeapLong_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_LeapLong_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_008b86d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b86d0_TEST FAIL\n");
    return 1;
}