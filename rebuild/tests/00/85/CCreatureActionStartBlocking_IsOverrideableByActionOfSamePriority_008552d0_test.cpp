#include <cstdio>
bool __fastcall CCreatureAction_StartBlocking_IsOverrideableByActionOfSamePriority()
{
    return false;
}
int main()
{
    if (CCreatureAction_StartBlocking_IsOverrideableByActionOfSamePriority() == false) { std::printf("AUTO_TINY_008552d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008552d0_TEST FAIL\n");
    return 1;
}