#include <cstdio>
int __fastcall AddChildPrimitive()
{
    return 1;
}
int main()
{
    if (AddChildPrimitive() == 1) { std::printf("AUTO_TINY_00b3b0a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b3b0a0_TEST FAIL\n");
    return 1;
}