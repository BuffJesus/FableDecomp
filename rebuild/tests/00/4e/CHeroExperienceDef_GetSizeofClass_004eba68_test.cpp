#include <cstdio>
int __fastcall CHeroExperienceDef_GetSizeofClass()
{
    return 180;
}
int main()
{
    if (CHeroExperienceDef_GetSizeofClass() == 180) { std::printf("AUTO_TINY_004eba68_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004eba68_TEST FAIL\n");
    return 1;
}