#include <cstdio>
int __fastcall CEngineInternalPrimitiveDecal_GetRenderLayerMask()
{
    return 16384;
}
int main()
{
    if (CEngineInternalPrimitiveDecal_GetRenderLayerMask() == 16384) { std::printf("AUTO_TINY_00bba330_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00bba330_TEST FAIL\n");
    return 1;
}