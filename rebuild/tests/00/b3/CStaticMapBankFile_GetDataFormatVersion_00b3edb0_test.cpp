#include <cstdio>
int __fastcall CStaticMapBankFile_GetDataFormatVersion()
{
    return 174;
}
int main()
{
    if (CStaticMapBankFile_GetDataFormatVersion() == 174) { std::printf("AUTO_TINY_00b3edb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b3edb0_TEST FAIL\n");
    return 1;
}