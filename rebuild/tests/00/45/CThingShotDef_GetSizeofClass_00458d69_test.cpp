#include <cstdio>
int __fastcall CThingShotDef_GetSizeofClass()
{
    return 172;
}
int main()
{
    if (CThingShotDef_GetSizeofClass() == 172) { std::printf("AUTO_TINY_00458d69_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00458d69_TEST FAIL\n");
    return 1;
}