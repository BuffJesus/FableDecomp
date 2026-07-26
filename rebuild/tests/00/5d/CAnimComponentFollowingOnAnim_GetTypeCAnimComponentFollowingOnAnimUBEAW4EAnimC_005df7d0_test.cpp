#include <cstdio>
int __fastcall CAnimComponentFollowingOnAnim_GetType()
{
    return 8;
}
int main()
{
    if (CAnimComponentFollowingOnAnim_GetType() == 8) { std::printf("AUTO_TINY_005df7d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df7d0_TEST FAIL\n");
    return 1;
}