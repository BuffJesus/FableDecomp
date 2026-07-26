#include <cstdio>
int __fastcall CSimBuildingDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CSimBuildingDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_00459700_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00459700_TEST FAIL\n");
    return 1;
}