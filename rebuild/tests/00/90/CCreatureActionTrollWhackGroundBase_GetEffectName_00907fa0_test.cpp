#include <cstdio>
bool __fastcall CCreatureAction_TrollWhackGroundBase_GetEffectName(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CCreatureAction_TrollWhackGroundBase_GetEffectName(&object, 1, 2)) { std::printf("AUTO_TINY_00907fa0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00907fa0_TEST FAIL\n");
    return 1;
}