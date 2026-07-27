#include <cstdio>
bool __fastcall CCombatActionBase_IsToFaceTargetAtStart()
{
    return false;
}
int main()
{
    if (CCombatActionBase_IsToFaceTargetAtStart() == false) { std::printf("AUTO_TINY_0062ea70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062ea70_TEST FAIL\n");
    return 1;
}