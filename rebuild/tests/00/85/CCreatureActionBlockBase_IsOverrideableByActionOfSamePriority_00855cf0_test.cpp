#include <cstdio>
bool __fastcall CCreatureAction_BlockBase_IsOverrideableByActionOfSamePriority()
{
    return true;
}
int main()
{
    if (CCreatureAction_BlockBase_IsOverrideableByActionOfSamePriority() == true) { std::printf("AUTO_TINY_00855cf0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00855cf0_TEST FAIL\n");
    return 1;
}