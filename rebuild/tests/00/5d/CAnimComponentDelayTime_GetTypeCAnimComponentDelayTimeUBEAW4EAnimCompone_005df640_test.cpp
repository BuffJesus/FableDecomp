#include <cstdio>
int __fastcall CAnimComponentDelayTime_GetType()
{
    return 2;
}
int main()
{
    if (CAnimComponentDelayTime_GetType() == 2) { std::printf("AUTO_TINY_005df640_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df640_TEST FAIL\n");
    return 1;
}