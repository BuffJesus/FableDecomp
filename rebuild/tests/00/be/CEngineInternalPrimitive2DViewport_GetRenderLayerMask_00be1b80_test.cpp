#include <cstdio>
int __fastcall CEngineInternalPrimitive2DViewport_GetRenderLayerMask()
{
    return 2147483648;
}
int main()
{
    if (CEngineInternalPrimitive2DViewport_GetRenderLayerMask() == 2147483648) { std::printf("AUTO_TINY_00be1b80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00be1b80_TEST FAIL\n");
    return 1;
}