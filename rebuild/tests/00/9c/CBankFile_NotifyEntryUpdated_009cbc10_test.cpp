#include <cstdio>
void __stdcall CBankFile_NotifyEntryUpdated(int arg0, int arg1, int arg2) {}
int main()
{
    CBankFile_NotifyEntryUpdated(1, 2, 3);
    std::printf("AUTO_TINY_009cbc10_TEST PASS\n");
    return 0;
}