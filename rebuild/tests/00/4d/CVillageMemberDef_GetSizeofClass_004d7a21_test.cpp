#include <cstdio>
int __fastcall CVillageMemberDef_GetSizeofClass()
{
    return 38;
}
int main()
{
    if (CVillageMemberDef_GetSizeofClass() == 38) { std::printf("AUTO_TINY_004d7a21_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d7a21_TEST FAIL\n");
    return 1;
}