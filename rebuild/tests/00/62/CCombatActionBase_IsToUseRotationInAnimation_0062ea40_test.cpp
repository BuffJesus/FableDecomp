#include <cstdio>
bool __fastcall CCombatActionBase_IsToUseRotationInAnimation()
{
    return true;
}
int main()
{
    if (CCombatActionBase_IsToUseRotationInAnimation() == true) { std::printf("AUTO_TINY_0062ea40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062ea40_TEST FAIL\n");
    return 1;
}