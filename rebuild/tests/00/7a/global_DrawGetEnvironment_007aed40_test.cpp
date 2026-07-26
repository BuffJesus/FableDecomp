#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 20);
}
int main()
{
    unsigned char object[24] = {0};
    *reinterpret_cast<int*>(object + 20) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_007aed40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_007aed40_TEST FAIL\n");
    return 1;
}