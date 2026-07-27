#include <cstdio>
bool __fastcall CCombatActionBase_IsToAlignWithGroundNormal()
{
    return false;
}
int main()
{
    if (CCombatActionBase_IsToAlignWithGroundNormal() == false) { std::printf("AUTO_TINY_0062ea50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062ea50_TEST FAIL\n");
    return 1;
}