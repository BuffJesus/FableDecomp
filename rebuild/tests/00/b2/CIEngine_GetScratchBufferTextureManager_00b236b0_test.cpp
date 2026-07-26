#include <cstdio>
int __fastcall CIEngine_GetScratchBufferTextureManager()
{
    return 0;
}
int main()
{
    if (CIEngine_GetScratchBufferTextureManager() == 0) { std::printf("AUTO_TINY_00b236b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b236b0_TEST FAIL\n");
    return 1;
}