#include <cstdio>
int __fastcall AddChildPrimitive()
{
    return 50;
}
int main()
{
    if (AddChildPrimitive() == 50) { std::printf("AUTO_TINY_007898f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007898f0_TEST FAIL\n");
    return 1;
}