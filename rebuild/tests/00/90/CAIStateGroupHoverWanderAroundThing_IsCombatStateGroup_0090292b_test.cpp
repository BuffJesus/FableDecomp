#include <cstdio>
bool __fastcall CAIStateGroup_HoverWanderAroundThing_IsCombatStateGroup()
{
    return false;
}
int main()
{
    if (CAIStateGroup_HoverWanderAroundThing_IsCombatStateGroup() == false) { std::printf("AUTO_TINY_0090292b_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0090292b_TEST FAIL\n");
    return 1;
}