#include <cstdio>
bool __fastcall CCreatureAction_Charge_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_Charge_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008b86f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b86f0_TEST FAIL\n");
    return 1;
}