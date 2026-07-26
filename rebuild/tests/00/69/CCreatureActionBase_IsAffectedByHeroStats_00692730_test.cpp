#include <cstdio>
bool __fastcall CCreatureActionBase_IsAffectedByHeroStats()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsAffectedByHeroStats() == false) { std::printf("AUTO_TINY_00692730_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692730_TEST FAIL\n");
    return 1;
}