#include <cstdio>
int __fastcall CKrakenTentacleDef_GetSizeofClass()
{
    return 96;
}
int main()
{
    if (CKrakenTentacleDef_GetSizeofClass() == 96) { std::printf("AUTO_TINY_004d5de7_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5de7_TEST FAIL\n");
    return 1;
}