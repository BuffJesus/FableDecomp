#include <cstdio>
int __fastcall CEngineInternalPrimitive2DAnimatedMesh_GetRenderLayerMask()
{
    return 2147483648;
}
int main()
{
    if (CEngineInternalPrimitive2DAnimatedMesh_GetRenderLayerMask() == 2147483648) { std::printf("AUTO_TINY_00bcea30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00bcea30_TEST FAIL\n");
    return 1;
}