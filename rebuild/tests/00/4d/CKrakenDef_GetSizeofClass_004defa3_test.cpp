#include <cstdio>
int __fastcall CKrakenDef_GetSizeofClass()
{
    return 124;
}
int main()
{
    if (CKrakenDef_GetSizeofClass() == 124) { std::printf("AUTO_TINY_004defa3_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004defa3_TEST FAIL\n");
    return 1;
}