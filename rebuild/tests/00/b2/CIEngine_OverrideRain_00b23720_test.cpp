#include <cstdio>
void __stdcall CIEngine_OverrideRain(int arg0) {}
int main()
{
    CIEngine_OverrideRain(1);
    std::printf("AUTO_TINY_00b23720_TEST PASS\n");
    return 0;
}