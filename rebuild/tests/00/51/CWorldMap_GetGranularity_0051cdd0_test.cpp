#include <cstdio>
int __fastcall CWorldMap_GetGranularity(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 80);
}
int main()
{
    unsigned char object[84] = {0};
    *reinterpret_cast<int*>(object + 80) = 0x12345678;
    if (CWorldMap_GetGranularity(object) == 0x12345678) { std::printf("AUTO_TINY_0051cdd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0051cdd0_TEST FAIL\n");
    return 1;
}