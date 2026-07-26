#include <cstdio>
int __fastcall CHasNameDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CHasNameDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004d66ac_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d66ac_TEST FAIL\n");
    return 1;
}