#include <cstdio>
int __fastcall CEngineInternalPrimitive2DSprite_GetRenderLayerMask(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 48);
}
int main()
{
    unsigned char object[52] = {0};
    *reinterpret_cast<int*>(object + 48) = 0x12345678;
    if (CEngineInternalPrimitive2DSprite_GetRenderLayerMask(object) == 0x12345678) { std::printf("AUTO_TINY_00baceb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00baceb0_TEST FAIL\n");
    return 1;
}