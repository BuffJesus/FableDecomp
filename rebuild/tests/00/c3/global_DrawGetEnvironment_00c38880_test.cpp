#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 372);
}
int main()
{
    unsigned char object[376] = {0};
    *reinterpret_cast<int*>(object + 372) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_00c38880_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00c38880_TEST FAIL\n");
    return 1;
}