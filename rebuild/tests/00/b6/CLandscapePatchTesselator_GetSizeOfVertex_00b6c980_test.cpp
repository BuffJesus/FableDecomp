#include <cstdio>
int __fastcall CLandscapePatchTesselator_GetSizeOfVertex()
{
    return 24;
}
int main()
{
    if (CLandscapePatchTesselator_GetSizeOfVertex() == 24) { std::printf("AUTO_TINY_00b6c980_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b6c980_TEST FAIL\n");
    return 1;
}