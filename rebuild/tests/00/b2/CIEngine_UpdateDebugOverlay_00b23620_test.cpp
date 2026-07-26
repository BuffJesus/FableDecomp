#include <cstdio>
void __stdcall CIEngine_UpdateDebugOverlay(int arg0, int arg1, int arg2) {}
int main()
{
    CIEngine_UpdateDebugOverlay(1, 2, 3);
    std::printf("AUTO_TINY_00b23620_TEST PASS\n");
    return 0;
}