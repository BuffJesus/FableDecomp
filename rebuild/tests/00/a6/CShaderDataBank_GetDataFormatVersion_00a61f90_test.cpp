#include <cstdio>
int __fastcall CShaderDataBank_GetDataFormatVersion()
{
    return 107;
}
int main()
{
    if (CShaderDataBank_GetDataFormatVersion() == 107) { std::printf("AUTO_TINY_00a61f90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a61f90_TEST FAIL\n");
    return 1;
}