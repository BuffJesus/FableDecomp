#include <cstdio>
bool __fastcall CWADFile_IsIOFinished()
{
    return true;
}
int main()
{
    if (CWADFile_IsIOFinished() == true) { std::printf("AUTO_TINY_00a39c30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a39c30_TEST FAIL\n");
    return 1;
}