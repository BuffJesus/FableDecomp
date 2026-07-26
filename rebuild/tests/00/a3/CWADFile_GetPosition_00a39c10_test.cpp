#include <cstdio>
int __fastcall CWADFile_GetPosition(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 12);
}
int main()
{
    unsigned char object[16] = {0};
    *reinterpret_cast<int*>(object + 12) = 0x12345678;
    if (CWADFile_GetPosition(object) == 0x12345678) { std::printf("AUTO_TINY_00a39c10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a39c10_TEST FAIL\n");
    return 1;
}