#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 8);
}
int main()
{
    unsigned char object[12] = {0};
    *reinterpret_cast<int*>(object + 8) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_00c61c60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00c61c60_TEST FAIL\n");
    return 1;
}