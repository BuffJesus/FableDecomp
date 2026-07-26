#include <cstdio>
bool __fastcall CCreatureAction_BlockRespond_IsOverrideableByActionOfSamePriority()
{
    return true;
}
int main()
{
    if (CCreatureAction_BlockRespond_IsOverrideableByActionOfSamePriority() == true) { std::printf("AUTO_TINY_00855d40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00855d40_TEST FAIL\n");
    return 1;
}