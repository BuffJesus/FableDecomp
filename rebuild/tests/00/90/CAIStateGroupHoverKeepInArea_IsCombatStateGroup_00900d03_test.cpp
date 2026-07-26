#include <cstdio>
bool __fastcall CAIStateGroup_HoverKeepInArea_IsCombatStateGroup()
{
    return true;
}
int main()
{
    if (CAIStateGroup_HoverKeepInArea_IsCombatStateGroup() == true) { std::printf("AUTO_TINY_00900d03_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900d03_TEST FAIL\n");
    return 1;
}