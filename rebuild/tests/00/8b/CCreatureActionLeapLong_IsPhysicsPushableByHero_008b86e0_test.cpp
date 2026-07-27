#include <cstdio>
bool __fastcall CCreatureAction_LeapLong_IsPhysicsPushableByHero()
{
    return false;
}
int main()
{
    if (CCreatureAction_LeapLong_IsPhysicsPushableByHero() == false) { std::printf("AUTO_TINY_008b86e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b86e0_TEST FAIL\n");
    return 1;
}