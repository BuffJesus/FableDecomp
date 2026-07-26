#include <cstdio>
int __fastcall CMultiStaticMeshEntryDef_GetSizeofClass()
{
    return 56;
}
int main()
{
    if (CMultiStaticMeshEntryDef_GetSizeofClass() == 56) { std::printf("AUTO_TINY_004eb89d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004eb89d_TEST FAIL\n");
    return 1;
}