#include <cstdio>
int __fastcall CHeroCentreDef_GetSizeofClass()
{
    return 37;
}
int main()
{
    if (CHeroCentreDef_GetSizeofClass() == 37) { std::printf("AUTO_TINY_004d463c_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d463c_TEST FAIL\n");
    return 1;
}