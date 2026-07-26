#include <cstdio>
int __fastcall AddChildPrimitive()
{
    return 2048;
}
int main()
{
    if (AddChildPrimitive() == 2048) { std::printf("AUTO_TINY_00997b30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00997b30_TEST FAIL\n");
    return 1;
}