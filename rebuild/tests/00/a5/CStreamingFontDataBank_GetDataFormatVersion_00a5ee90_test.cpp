#include <cstdio>
int __fastcall CStreamingFontDataBank_GetDataFormatVersion()
{
    return 109;
}
int main()
{
    if (CStreamingFontDataBank_GetDataFormatVersion() == 109) { std::printf("AUTO_TINY_00a5ee90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a5ee90_TEST FAIL\n");
    return 1;
}