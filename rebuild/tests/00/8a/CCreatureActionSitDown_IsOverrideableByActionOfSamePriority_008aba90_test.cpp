#include <cstdio>
bool __fastcall CCreatureAction_SitDown_IsOverrideableByActionOfSamePriority()
{
    return true;
}
int main()
{
    if (CCreatureAction_SitDown_IsOverrideableByActionOfSamePriority() == true) { std::printf("AUTO_TINY_008aba90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008aba90_TEST FAIL\n");
    return 1;
}