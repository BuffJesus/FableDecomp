#include <cstdio>
int __fastcall CMeshDataBank_GetDataFormatVersion()
{
    return 205;
}
int main()
{
    if (CMeshDataBank_GetDataFormatVersion() == 205) { std::printf("AUTO_TINY_00a23ac0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a23ac0_TEST FAIL\n");
    return 1;
}