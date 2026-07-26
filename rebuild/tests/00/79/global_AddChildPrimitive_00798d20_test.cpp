#include <cstdio>
int __fastcall AddChildPrimitive()
{
    return 10;
}
int main()
{
    if (AddChildPrimitive() == 10) { std::printf("AUTO_TINY_00798d20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00798d20_TEST FAIL\n");
    return 1;
}