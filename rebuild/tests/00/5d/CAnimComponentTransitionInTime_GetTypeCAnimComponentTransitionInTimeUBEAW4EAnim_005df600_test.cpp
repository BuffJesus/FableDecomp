#include <cstdio>
int __fastcall CAnimComponentTransitionInTime_GetType()
{
    return 1;
}
int main()
{
    if (CAnimComponentTransitionInTime_GetType() == 1) { std::printf("AUTO_TINY_005df600_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df600_TEST FAIL\n");
    return 1;
}