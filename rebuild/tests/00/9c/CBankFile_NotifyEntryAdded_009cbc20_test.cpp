#include <cstdio>
void __stdcall CBankFile_NotifyEntryAdded(int arg0, int arg1, int arg2) {}
int main()
{
    CBankFile_NotifyEntryAdded(1, 2, 3);
    std::printf("AUTO_TINY_009cbc20_TEST PASS\n");
    return 0;
}