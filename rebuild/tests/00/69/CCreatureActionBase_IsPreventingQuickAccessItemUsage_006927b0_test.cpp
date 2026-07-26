#include <cstdio>
bool __fastcall CCreatureActionBase_IsPreventingQuickAccessItemUsage()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsPreventingQuickAccessItemUsage() == false) { std::printf("AUTO_TINY_006927b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006927b0_TEST FAIL\n");
    return 1;
}