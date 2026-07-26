#include <cstdio>
bool __fastcall CCreatureAction_UseObject_IsOverrideableByActionOfSamePriority()
{
    return true;
}
int main()
{
    if (CCreatureAction_UseObject_IsOverrideableByActionOfSamePriority() == true) { std::printf("AUTO_TINY_006a0850_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a0850_TEST FAIL\n");
    return 1;
}