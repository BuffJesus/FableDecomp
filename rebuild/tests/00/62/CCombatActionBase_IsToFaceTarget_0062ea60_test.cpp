#include <cstdio>
bool __fastcall CCombatActionBase_IsToFaceTarget()
{
    return false;
}
int main()
{
    if (CCombatActionBase_IsToFaceTarget() == false) { std::printf("AUTO_TINY_0062ea60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062ea60_TEST FAIL\n");
    return 1;
}