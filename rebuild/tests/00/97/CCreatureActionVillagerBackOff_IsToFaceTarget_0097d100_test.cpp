#include <cstdio>
bool __fastcall CCreatureAction_VillagerBackOff_IsToFaceTarget()
{
    return true;
}
int main()
{
    if (CCreatureAction_VillagerBackOff_IsToFaceTarget() == true) { std::printf("AUTO_TINY_0097d100_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0097d100_TEST FAIL\n");
    return 1;
}