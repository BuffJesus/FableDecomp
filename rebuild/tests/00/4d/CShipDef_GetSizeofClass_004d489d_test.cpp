#include <cstdio>
int __fastcall CShipDef_GetSizeofClass()
{
    return 68;
}
int main()
{
    if (CShipDef_GetSizeofClass() == 68) { std::printf("AUTO_TINY_004d489d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d489d_TEST FAIL\n");
    return 1;
}