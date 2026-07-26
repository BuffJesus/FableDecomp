#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 156);
}
int main()
{
    unsigned char object[160] = {0};
    *reinterpret_cast<int*>(object + 156) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_004fb150_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004fb150_TEST FAIL\n");
    return 1;
}