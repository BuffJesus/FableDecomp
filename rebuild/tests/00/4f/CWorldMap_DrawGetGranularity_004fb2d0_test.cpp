#include <cstdio>
int __fastcall CWorldMap_DrawGetGranularity(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 76);
}
int main()
{
    unsigned char object[80] = {0};
    *reinterpret_cast<int*>(object + 76) = 0x12345678;
    if (CWorldMap_DrawGetGranularity(object) == 0x12345678) { std::printf("AUTO_TINY_004fb2d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004fb2d0_TEST FAIL\n");
    return 1;
}