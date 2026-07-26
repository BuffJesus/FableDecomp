#include <cstdio>
int __fastcall CFontDataBank_GetDataFormatVersion()
{
    return 105;
}
int main()
{
    if (CFontDataBank_GetDataFormatVersion() == 105) { std::printf("AUTO_TINY_00a63340_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a63340_TEST FAIL\n");
    return 1;
}