#include <cstdio>
int __fastcall CAnimatingObjectDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CAnimatingObjectDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_004ea20a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004ea20a_TEST FAIL\n");
    return 1;
}