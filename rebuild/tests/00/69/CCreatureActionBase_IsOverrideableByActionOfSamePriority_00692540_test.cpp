#include <cstdio>
bool __fastcall CCreatureActionBase_IsOverrideableByActionOfSamePriority()
{
    return false;
}
int main()
{
    if (CCreatureActionBase_IsOverrideableByActionOfSamePriority() == false) { std::printf("AUTO_TINY_00692540_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00692540_TEST FAIL\n");
    return 1;
}