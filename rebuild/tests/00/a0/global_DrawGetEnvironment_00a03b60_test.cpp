#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 36);
}
int main()
{
    unsigned char object[40] = {0};
    *reinterpret_cast<int*>(object + 36) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_00a03b60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a03b60_TEST FAIL\n");
    return 1;
}