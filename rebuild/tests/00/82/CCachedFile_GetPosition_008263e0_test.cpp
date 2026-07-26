#include <cstdio>
int __fastcall CCachedFile_GetPosition(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 4);
}
int main()
{
    unsigned char object[8] = {0};
    *reinterpret_cast<int*>(object + 4) = 0x12345678;
    if (CCachedFile_GetPosition(object) == 0x12345678) { std::printf("AUTO_TINY_008263e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008263e0_TEST FAIL\n");
    return 1;
}