#include <cstdio>
void __stdcall CIEngine_EnableDebugOverlay(int arg0, int arg1, int arg2) {}
int main()
{
    CIEngine_EnableDebugOverlay(1, 2, 3);
    std::printf("AUTO_TINY_00b23630_TEST PASS\n");
    return 0;
}