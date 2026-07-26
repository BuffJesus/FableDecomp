#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 376);
}
int main()
{
    unsigned char object[380] = {0};
    *reinterpret_cast<int*>(object + 376) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_00c38870_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00c38870_TEST FAIL\n");
    return 1;
}