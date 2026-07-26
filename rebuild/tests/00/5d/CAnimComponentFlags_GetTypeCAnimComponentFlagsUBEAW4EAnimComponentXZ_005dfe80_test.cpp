#include <cstdio>
int __fastcall CAnimComponentFlags_GetType()
{
    return 0;
}
int main()
{
    if (CAnimComponentFlags_GetType() == 0) { std::printf("AUTO_TINY_005dfe80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005dfe80_TEST FAIL\n");
    return 1;
}