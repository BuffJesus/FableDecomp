#include <cstdio>
int __fastcall CMultiStaticMeshDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CMultiStaticMeshDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004e1533_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e1533_TEST FAIL\n");
    return 1;
}