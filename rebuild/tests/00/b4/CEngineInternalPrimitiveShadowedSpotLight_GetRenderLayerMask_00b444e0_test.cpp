#include <cstdio>
int __fastcall CEngineInternalPrimitiveShadowedSpotLight_GetRenderLayerMask()
{
    return 1;
}
int main()
{
    if (CEngineInternalPrimitiveShadowedSpotLight_GetRenderLayerMask() == 1) { std::printf("AUTO_TINY_00b444e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b444e0_TEST FAIL\n");
    return 1;
}