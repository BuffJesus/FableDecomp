#include <cstdio>
int __fastcall CGraphicDataBank_GetDataFormatVersion()
{
    return 122;
}
int main()
{
    if (CGraphicDataBank_GetDataFormatVersion() == 122) { std::printf("AUTO_TINY_009fd0b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009fd0b0_TEST FAIL\n");
    return 1;
}