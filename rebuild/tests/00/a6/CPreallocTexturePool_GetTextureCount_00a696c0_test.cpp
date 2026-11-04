#include <cstdio>
int __fastcall CPreallocTexturePool_GetTextureCount(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 60);
}
int main()
{
    unsigned char object[64] = {0};
    *reinterpret_cast<int*>(object + 60) = 0x12345678;
    if (CPreallocTexturePool_GetTextureCount(object) == 0x12345678) { std::printf("AUTO_TINY_00a696c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a696c0_TEST FAIL\n");
    return 1;
}