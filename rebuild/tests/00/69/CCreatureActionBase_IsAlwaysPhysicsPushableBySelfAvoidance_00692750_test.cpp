#include <cstdio>
bool __fastcall CCreatureActionBase_IsAlwaysPhysicsPushableBySelfAvoidance()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsAlwaysPhysicsPushableBySelfAvoidance() == false) { std::printf("AUTO_TINY_00692750_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692750_TEST FAIL\n");
    return 1;
}