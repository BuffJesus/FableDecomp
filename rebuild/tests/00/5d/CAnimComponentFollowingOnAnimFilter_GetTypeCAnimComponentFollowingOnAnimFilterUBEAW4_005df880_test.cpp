#include <cstdio>
int __fastcall CAnimComponentFollowingOnAnimFilter_GetType()
{
    return 11;
}
int main()
{
    if (CAnimComponentFollowingOnAnimFilter_GetType() == 11) { std::printf("AUTO_TINY_005df880_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df880_TEST FAIL\n");
    return 1;
}