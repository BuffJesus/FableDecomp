#include <cstdio>
int __fastcall CMap_DrawGetHeightMapSizeY(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 7448);
}
int main()
{
    unsigned char object[7452] = {0};
    *reinterpret_cast<int*>(object + 7448) = 0x12345678;
    if (CMap_DrawGetHeightMapSizeY(object) == 0x12345678) { std::printf("AUTO_TINY_00827860_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00827860_TEST FAIL\n");
    return 1;
}