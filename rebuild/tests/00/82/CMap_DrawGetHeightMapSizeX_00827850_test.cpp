#include <cstdio>
int __fastcall CMap_DrawGetHeightMapSizeX(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 7444);
}
int main()
{
    unsigned char object[7448] = {0};
    *reinterpret_cast<int*>(object + 7444) = 0x12345678;
    if (CMap_DrawGetHeightMapSizeX(object) == 0x12345678) { std::printf("AUTO_TINY_00827850_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00827850_TEST FAIL\n");
    return 1;
}