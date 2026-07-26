#include <cstdio>
int __fastcall CExperienceDef_GetSizeofClass()
{
    return 80;
}
int main()
{
    if (CExperienceDef_GetSizeofClass() == 80) { std::printf("AUTO_TINY_004e088a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e088a_TEST FAIL\n");
    return 1;
}