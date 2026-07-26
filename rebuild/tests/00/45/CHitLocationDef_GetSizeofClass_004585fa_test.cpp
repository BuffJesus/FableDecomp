#include <cstdio>
int __fastcall CHitLocationDef_GetSizeofClass()
{
    return 124;
}
int main()
{
    if (CHitLocationDef_GetSizeofClass() == 124) { std::printf("AUTO_TINY_004585fa_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004585fa_TEST FAIL\n");
    return 1;
}