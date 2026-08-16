#include <cstdio>
int __fastcall CAIStateGroup_MinionFollowThing_GetStateGroupType()
{
    return 2;
}
int main()
{
    if (CAIStateGroup_MinionFollowThing_GetStateGroupType() == 2) { std::printf("AUTO_TINY_009014b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009014b0_TEST FAIL\n");
    return 1;
}