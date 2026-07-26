#include <cstdio>
int __fastcall CBufferedFileDataOutputStream_GetPosition(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 32);
}
int main()
{
    unsigned char object[36] = {0};
    *reinterpret_cast<int*>(object + 32) = 0x12345678;
    if (CBufferedFileDataOutputStream_GetPosition(object) == 0x12345678) { std::printf("AUTO_TINY_00994510_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00994510_TEST FAIL\n");
    return 1;
}