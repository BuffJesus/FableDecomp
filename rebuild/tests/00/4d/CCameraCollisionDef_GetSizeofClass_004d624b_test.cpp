#include <cstdio>
int __fastcall CCameraCollisionDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CCameraCollisionDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d624b_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d624b_TEST FAIL\n");
    return 1;
}