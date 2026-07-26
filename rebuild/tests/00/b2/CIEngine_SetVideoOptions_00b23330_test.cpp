#include <cstdio>
bool __fastcall CIEngine_SetVideoOptions(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CIEngine_SetVideoOptions(&object, 1, 2)) { std::printf("AUTO_TINY_00b23330_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b23330_TEST FAIL\n");
    return 1;
}