#include <cstdio>
int __fastcall CVillageDef_GetSizeofClass()
{
    return 268;
}
int main()
{
    if (CVillageDef_GetSizeofClass() == 268) { std::printf("AUTO_TINY_004dff9f_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004dff9f_TEST FAIL\n");
    return 1;
}