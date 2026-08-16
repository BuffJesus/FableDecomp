#include <cstdio>
int __fastcall CMovableResource_Verify()
{
    return 123;
}
int main()
{
    if (CMovableResource_Verify() == 123) { std::printf("AUTO_TINY_009d5100_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009d5100_TEST FAIL\n");
    return 1;
}