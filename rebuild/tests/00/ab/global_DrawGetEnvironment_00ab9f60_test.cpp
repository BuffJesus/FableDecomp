#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 44);
}
int main()
{
    unsigned char object[48] = {0};
    *reinterpret_cast<int*>(object + 44) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_00ab9f60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00ab9f60_TEST FAIL\n");
    return 1;
}