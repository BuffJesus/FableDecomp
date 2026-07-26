#include <cstdio>
int __fastcall CEngineInternalPrimitiveLight_GetRenderLayerMask()
{
    return 1;
}
int main()
{
    if (CEngineInternalPrimitiveLight_GetRenderLayerMask() == 1) { std::printf("AUTO_TINY_00b444d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b444d0_TEST FAIL\n");
    return 1;
}