#include <cstdio>
int __fastcall CEngineInternalPrimitive2DClockSprite_GetRenderLayerMask()
{
    return 2147483648;
}
int main()
{
    if (CEngineInternalPrimitive2DClockSprite_GetRenderLayerMask() == 2147483648) { std::printf("AUTO_TINY_00bacec0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00bacec0_TEST FAIL\n");
    return 1;
}