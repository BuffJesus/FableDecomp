#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 24);
}
int main()
{
    unsigned char object[28] = {0};
    *reinterpret_cast<int*>(object + 24) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_00814be0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00814be0_TEST FAIL\n");
    return 1;
}