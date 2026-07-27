#include <cstdio>
bool __fastcall CIEngine_DeallocateShadowBuffer()
{
    return false;
}
int main()
{
    if (CIEngine_DeallocateShadowBuffer() == false) { std::printf("AUTO_TINY_00b236c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b236c0_TEST FAIL\n");
    return 1;
}