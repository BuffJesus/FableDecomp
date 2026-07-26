#include <cstdio>
int __fastcall CThingGroupDef_GetSizeofClass()
{
    return 58;
}
int main()
{
    if (CThingGroupDef_GetSizeofClass() == 58) { std::printf("AUTO_TINY_00459549_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00459549_TEST FAIL\n");
    return 1;
}