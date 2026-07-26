#include <cstdio>
void __stdcall CIEngine_SetGlobalLightingParameters(int arg0, int arg1, int arg2, int arg3) {}
int main()
{
    CIEngine_SetGlobalLightingParameters(1, 2, 3, 4);
    std::printf("AUTO_TINY_00b23640_TEST PASS\n");
    return 0;
}