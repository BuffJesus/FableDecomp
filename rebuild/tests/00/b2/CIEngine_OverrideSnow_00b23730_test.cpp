#include <cstdio>
void __stdcall CIEngine_OverrideSnow(int arg0) {}
int main()
{
    CIEngine_OverrideSnow(1);
    std::printf("AUTO_TINY_00b23730_TEST PASS\n");
    return 0;
}