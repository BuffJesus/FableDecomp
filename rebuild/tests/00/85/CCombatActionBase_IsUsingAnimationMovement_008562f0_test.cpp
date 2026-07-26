#include <cstdio>
bool __fastcall CCombatActionBase_IsUsingAnimationMovement()
{
    return true;
}
int main()
{
    if (CCombatActionBase_IsUsingAnimationMovement() == true) { std::printf("AUTO_TINY_008562f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008562f0_TEST FAIL\n");
    return 1;
}