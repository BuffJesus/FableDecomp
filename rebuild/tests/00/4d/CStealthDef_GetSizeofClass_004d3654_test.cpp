#include <cstdio>
int __fastcall CStealthDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CStealthDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_004d3654_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d3654_TEST FAIL\n");
    return 1;
}