#include <cstdio>
int __fastcall CReplaceableMeshDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CReplaceableMeshDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004e3e69_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e3e69_TEST FAIL\n");
    return 1;
}