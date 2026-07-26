#include <cstdio>
int __fastcall CMemoryDataOutputStream_GetPosition(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 20);
}
int main()
{
    unsigned char object[24] = {0};
    *reinterpret_cast<int*>(object + 20) = 0x12345678;
    if (CMemoryDataOutputStream_GetPosition(object) == 0x12345678) { std::printf("AUTO_TINY_009945b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009945b0_TEST FAIL\n");
    return 1;
}