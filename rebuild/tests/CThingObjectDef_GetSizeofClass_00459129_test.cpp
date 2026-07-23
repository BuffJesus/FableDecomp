#include <cstdio>
int __fastcall CThingObjectDef_GetSizeofClass()
{
    return 156;
}
int main()
{
    if (CThingObjectDef_GetSizeofClass() == 156) { std::printf("AUTO_TINY_00459129_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00459129_TEST FAIL\n");
    return 1;
}