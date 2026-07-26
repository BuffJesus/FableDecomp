#include <cstdio>
bool __fastcall CCreatureAction_NymphSummonInto_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_NymphSummonInto_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008ca8d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ca8d0_TEST FAIL\n");
    return 1;
}