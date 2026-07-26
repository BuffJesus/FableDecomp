#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 28);
}
int main()
{
    unsigned char object[32] = {0};
    *reinterpret_cast<int*>(object + 28) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_006925c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006925c0_TEST FAIL\n");
    return 1;
}