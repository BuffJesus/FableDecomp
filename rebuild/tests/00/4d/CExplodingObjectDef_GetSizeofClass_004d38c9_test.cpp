#include <cstdio>
int __fastcall CExplodingObjectDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CExplodingObjectDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004d38c9_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d38c9_TEST FAIL\n");
    return 1;
}