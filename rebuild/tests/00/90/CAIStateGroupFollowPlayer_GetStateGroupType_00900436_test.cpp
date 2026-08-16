#include <cstdio>
int __fastcall CAIStateGroup_FollowPlayer_GetStateGroupType()
{
    return 2;
}
int main()
{
    if (CAIStateGroup_FollowPlayer_GetStateGroupType() == 2) { std::printf("AUTO_TINY_00900436_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00900436_TEST FAIL\n");
    return 1;
}