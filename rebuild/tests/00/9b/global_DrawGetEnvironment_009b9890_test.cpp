#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 4);
}
int main()
{
    unsigned char object[8] = {0};
    *reinterpret_cast<int*>(object + 4) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_009b9890_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009b9890_TEST FAIL\n");
    return 1;
}