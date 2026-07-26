#include <cstdio>
void __stdcall CBankFile_NotifyBankSize(int arg0, int arg1) {}
int main()
{
    CBankFile_NotifyBankSize(1, 2);
    std::printf("AUTO_TINY_009cbbf0_TEST PASS\n");
    return 0;
}