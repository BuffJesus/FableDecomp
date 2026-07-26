#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 184);
}
int main()
{
    unsigned char object[188] = {0};
    *reinterpret_cast<int*>(object + 184) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_005bc4c4_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005bc4c4_TEST FAIL\n");
    return 1;
}