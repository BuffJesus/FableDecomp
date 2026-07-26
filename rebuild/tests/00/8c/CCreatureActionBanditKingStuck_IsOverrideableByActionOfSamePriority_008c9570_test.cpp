#include <cstdio>
bool __fastcall CCreatureAction_BanditKingStuck_IsOverrideableByActionOfSamePriority()
{
    return true;
}
int main()
{
    if (CCreatureAction_BanditKingStuck_IsOverrideableByActionOfSamePriority() == true) { std::printf("AUTO_TINY_008c9570_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c9570_TEST FAIL\n");
    return 1;
}